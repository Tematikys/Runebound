#include "game.hpp"
#include <filesystem>
#include <nlohmann/json.hpp>
#include "card_fight.hpp"
#include "point.hpp"
#include "product.hpp"
#include "runebound_fwd.hpp"
#include "skill_card.hpp"

namespace runebound {
namespace game {

/*
void to_json(nlohmann::json &json, const Game &game) {
    json["m_map"] = game.m_map;
    json["m_characters"] = game.m_characters;
    json["m_card_deck_research"] = game.m_card_deck_research;
    json["m_tokens"] = game.m_tokens;
    json["m_turn"] = game.m_turn;
    json["m_count_players"] = game.m_count_players;
    json["ALL_CARDS_RESEARCH"] = game.ALL_CARDS_RESEARCH;
}

void from_json(const nlohmann::json &json, Game &game) {
    int count_players = json["M_COUNT_PLAYERS"];
    std::vector<unsigned int> cards;
    for (const nlohmann::json &card : json["m_card_deck_research"]) {
        cards.push_back(card);
    }
    game.m_card_deck_research = cards;
    game.m_count_players = json["m_count_players"];
    game.m_map = json["m_map"];
    game.m_characters = json["m_characters"];
    game.m_tokens = json["m_tokens"];
    game.m_turn = json["m_turn"];
}
*/
Point Game::get_position_character(
    const std::shared_ptr<character::Character> &chr
) const {
    return chr->get_position();
}

bool Game::check_characteristic_private(
    int number_attempts,
    Characteristic characteristic
) {
    for (int i = 0; i < number_attempts; ++i) {
        if (m_card_deck_skill.empty()) {
            generate_all_skill_cards();
        }
        auto card = m_card_deck_skill.back();
        m_card_deck_skill.pop_back();
        m_last_characteristic_check.push_back(card);
        if (m_all_skill_cards[card].check_success()) {
            return true;
        }
    }
    return false;
}

std::shared_ptr<::runebound::character::Character> Game::make_character(
    const ::runebound::character::StandardCharacter &name
) {
    if (m_remaining_standard_characters.count(name) == 0) {
        throw CharacterAlreadySelected();
    }
    m_characters.emplace_back(
        std::make_shared<::runebound::character::Character>(
            ::runebound::character::Character(name)
        )
    );
    m_count_players += 1;
    m_remaining_standard_characters.erase(name);
    return m_characters.back();
}

void Game::generate_all_skill_cards() {
    m_card_deck_skill.resize(100);
    for (int i = 0; i < 100; ++i) {
        m_card_deck_skill[i] = m_all_skill_cards.size();
        m_all_skill_cards.emplace_back(cards::SkillCard(
            static_cast<bool>(rng() % 2),
            static_cast<Characteristic>(rng() % 3),
            static_cast<int>(rng() % 3) + 1
        ));
    }
}

void Game::generate_all_cards_research() {
    m_card_deck_research.resize(DECK_SIZE);
    std::string path = "data/json/cards/cards_research";
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        nlohmann::json json;
        std::ifstream in(entry.path());
        in >> json;
        cards::CardResearch card;
        ::runebound::cards::from_json(json, card, m_map);
        m_all_cards_research.push_back(card);
    }
    for (int i = 0; i < DECK_SIZE; ++i) {
        m_card_deck_research[i] = i;
    }
}

void Game::generate_all_cards_fight() {
    m_card_deck_fight.resize(DECK_SIZE);
    std::string path = "data/json/cards/cards_fight";
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        nlohmann::json json;
        std::ifstream in(entry.path());
        in >> json;
        cards::CardFight card;
        ::runebound::cards::from_json(json, card);
        m_all_cards_fight.push_back(card);
    }
    for (int i = 0; i < DECK_SIZE; ++i) {
        m_card_deck_fight[i] = i;
    }
}

void Game::generate_all_products() {
    std::string path = "data/json/products";
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        nlohmann::json json;
        std::ifstream in(entry.path());
        in >> json;
        trade::Product product;
        ::runebound::trade::from_json(json, product);
        m_all_products.push_back(product);
    }
    m_remaining_products.resize(m_all_products.size());
    for (std::size_t i = 0; i < m_all_products.size(); ++i) {
        m_remaining_products[i] = i;
    }
}

void Game::generate_all_cards_meeting() {
    m_card_deck_meeting.resize(DECK_SIZE);
    std::string path = "data/json/cards/cards_meeting";
    for (const auto &entry : std::filesystem::directory_iterator(path)) {
        nlohmann::json json;
        std::ifstream in(entry.path());
        in >> json;
        cards::CardMeeting card;
        ::runebound::cards::from_json(json, card);
        m_all_cards_meeting.push_back(card);
    }
    for (int i = 0; i < DECK_SIZE; ++i) {
        m_card_deck_meeting[i] = i;
    }
}

void Game::generate_all_shops() {
    auto towns = m_map.get_towns();
    for (const auto &town : towns) {
        m_shops[town] = {};
        for (int i = 0; i < 3; ++i) {
            auto product =
                m_remaining_products[rng() % m_remaining_products.size()];
            m_shops[town].insert(product);
            m_remaining_products.erase(std::find(
                m_remaining_products.begin(), m_remaining_products.end(),
                product
            ));
        }
    }
}

void Game::relax(std::shared_ptr<character::Character> chr) {
    check_turn(chr);
    check_sufficiency_action_points(1);
    m_characters[m_turn]->relax();
    m_characters[m_turn]->update_action_points(-1);
}

void Game::take_token(const std::shared_ptr<character::Character> &chr) {
    check_turn(chr);
    check_sufficiency_action_points(2);
    Point position = chr->get_position();
    if (m_map.get_cell_map(position).get_token() == AdventureType::NOTHING) {
        throw NoTokenException();
    }
    if (m_map.get_cell_map(position).get_side_token() == Side::BACK) {
        throw BackSideTokenException();
    }
    if (m_map.get_cell_map(position).get_token() == AdventureType::FIGHT) {
        unsigned int card = m_card_deck_fight[rng() % m_card_deck_fight.size()];
        chr->add_card(AdventureType::FIGHT, card);
        m_card_deck_fight.erase(
            std::find(m_card_deck_fight.begin(), m_card_deck_fight.end(), card)
        );
        chr->start_fight(std::make_shared<fight::Fight>(
            chr, m_all_cards_fight[card].get_enemy()
        ));
        m_current_fight = chr->get_current_fight();
        m_characters[(m_turn + m_count_players - 1) % m_count_players]
            ->start_fight_as_enemy();
    } else if (m_map.get_cell_map(position).get_token() == AdventureType::RESEARCH) {
        unsigned int card =
            m_card_deck_research[rng() % m_card_deck_research.size()];
        chr->add_card(AdventureType::RESEARCH, card);
        m_card_deck_research.erase(std::find(
            m_card_deck_research.begin(), m_card_deck_research.end(), card
        ));
    } else if (m_map.get_cell_map(position).get_token() == AdventureType::MEETING) {
        unsigned int card =
            m_card_deck_meeting[rng() % m_card_deck_meeting.size()];
        chr->add_card(AdventureType::MEETING, card);
        m_card_deck_meeting.erase(std::find(
            m_card_deck_meeting.begin(), m_card_deck_meeting.end(), card
        ));
    } else {
        chr->start_fight(std::make_shared<fight::Fight>(
            chr, fight::Enemy(AdventureType::BOSS)
        ));
        m_current_fight = chr->get_current_fight();
        m_characters[(m_turn + m_count_players - 1) % m_count_players]
            ->start_fight_as_enemy();
        m_map.reverse_token(position);
    }
    m_map.reverse_token(position);
    m_characters[m_turn]->update_action_points(-2);
}

void Game::end_fight_with_boss(const std::shared_ptr<character::Character> &chr
) {
    if (chr->get_current_fight() == nullptr) {
        throw NoFight();
    }
    if (chr->get_current_fight()->get_winner() ==
        fight::Participant::CHARACTER) {
        m_game_over = true;
        m_winner = chr->get_standard_character();
    }
    chr->end_fight_with_boss();
    m_current_fight = nullptr;
    m_characters[(m_turn + m_count_players - 1) % m_count_players]
        ->end_fight_as_enemy();
}

void Game::end_fight(const std::shared_ptr<character::Character> &chr) {
    if (chr->get_cards(AdventureType::FIGHT).empty()) {
        throw NoCardFight();
    }
    if (chr->get_current_fight() == nullptr) {
        throw NoFight();
    }
    if (chr->get_current_fight()->get_winner() ==
        fight::Participant::CHARACTER) {
        chr->change_gold(
            m_all_cards_fight[chr->get_card_fight()].get_gold_award()
        );
        chr->add_trophy(AdventureType::FIGHT, chr->get_card_fight());
    }
    chr->end_fight();
    m_current_fight = nullptr;
    m_characters[(m_turn + m_count_players - 1) % m_count_players]
        ->end_fight_as_enemy();
}

std::vector<Point> Game::make_move(
    const std::shared_ptr<character::Character> &chr,
    const Point &end,
    std::vector<::runebound::dice::HandDice> &dice_roll_results
) {
    check_turn(chr);
    if (m_last_dice_movement_result.empty() &&
        m_map.check_neighbour(m_characters[m_turn]->get_position(), end)) {
        check_sufficiency_action_points(1);
        m_characters[m_turn]->set_position(end);
        chr->update_action_points(-1);
        return {m_characters[m_turn]->get_position(), end};
    }
    if (m_last_dice_movement_result.empty()) {
        throw NonThrownDiceException();
    }
    std::vector<Point> result = m_map.check_move(
        m_characters[m_turn]->get_position(), end, dice_roll_results
    );
    if (result.empty()) {
        throw InaccessibleMoveException();
    }
    m_characters[m_turn]->set_position(end);
    m_last_dice_movement_result.clear();
    return result;
}

std::vector<std::size_t> Game::get_possible_outcomes(
    const std::shared_ptr<character::Character> &chr
) {
    std::vector<std::size_t> outcomes;
    auto card = chr->get_active_card_research();
    for (std::size_t i = 0;
         i < m_all_cards_research[card].get_outcomes().size(); ++i) {
        if (m_all_cards_research[card].check_outcome(
                i, m_last_dice_research_result
            )) {
            outcomes.push_back(i);
        }
    }
    m_last_possible_outcomes = outcomes;
    return outcomes;
}

void Game::complete_card_research(
    const std::shared_ptr<character::Character> &chr,
    int desired_outcome
) {
    auto card = chr->get_active_card_research();
    if (desired_outcome < 0) {
        chr->pop_card(AdventureType::RESEARCH, card);
        m_last_dice_research_result.clear();
        return;
    }
    if (!m_all_cards_research[card].check_outcome(
            desired_outcome, m_last_dice_research_result
        )) {
        throw BadOutcomeException();
    }
    chr->add_trophy(AdventureType::RESEARCH, card);
    chr->update_health(
        m_all_cards_research[card].get_delta_health(desired_outcome)
    );
    chr->change_gold(m_all_cards_research[card].get_delta_gold(desired_outcome)
    );
    chr->change_knowledge_token(
        m_all_cards_research[card].get_knowledge_token(desired_outcome)
    );
    chr->pop_card(AdventureType::RESEARCH, card);
    m_last_dice_research_result.clear();
    m_last_possible_outcomes.clear();
}

bool Game::check_characteristic(
    const std::shared_ptr<character::Character> &chr,
    unsigned int card,
    cards::OptionMeeting option
) {
    check_turn(chr);
    m_last_characteristic_check.clear();
    int number_attempts =
        chr->get_characteristic(
            m_all_cards_meeting[card].get_verifiable_characteristic(option)
        ) +
        m_all_cards_meeting[card].get_change_characteristic(option);
    chr->pop_card(AdventureType::MEETING, card);
    if (check_characteristic_private(
            number_attempts,
            m_all_cards_meeting[card].get_verifiable_characteristic(option)
        )) {
        chr->change_gold(m_all_cards_meeting[card].get_gold_award(option));
        chr->change_knowledge_token(
            m_all_cards_meeting[card].get_knowledge_token(option)
        );
        chr->add_trophy(AdventureType::MEETING, card);
        return true;
    }
    return false;
}

void Game::start_trade(const std::shared_ptr<character::Character> &chr) {
    check_turn(chr);
    check_town_location(chr);
    check_sufficiency_action_points(1);
    chr->update_action_points(-1);
    add_product_to_shop(chr->get_position());
    chr->start_trade();
}

void Game::end_trade(const std::shared_ptr<character::Character> &chr) {
    check_turn(chr);
    chr->end_trade();
}

void Game::sell_product_in_town(
    const std::shared_ptr<character::Character> &chr,
    unsigned int product
) {
    check_turn(chr);
    if (!chr->check_product(product)) {
        throw NoProductException();
    }
    if (m_all_products[product].get_place_of_cell() !=
        map::SpecialTypeCell::NOTHING) {
        throw NoProductSaleException();
    }
    check_town_location(chr);
    m_all_products[product].undo_product(chr);
    chr->erase_product(product);
    m_remaining_products.push_back(product);
    chr->change_gold(static_cast<int>(m_all_products[product].get_market_price()
    ));
}

void Game::buy_product(
    const std::shared_ptr<character::Character> &chr,
    unsigned int product
) {
    check_turn(chr);
    check_town_location(chr);
    if (m_shops[chr->get_position()].count(product) == 0) {
        throw NoProductException();
    }
    if (chr->get_gold() < m_all_products[product].get_price()) {
        throw NotEnoughGoldException();
    }
    remove_product_from_shop(chr->get_position(), product);
    m_all_products[product].apply_product(chr);
    chr->add_product(product);
    chr->change_gold(-static_cast<int>(m_all_products[product].get_price()));
    end_trade(chr);
}

void Game::discard_product(
    const std::shared_ptr<character::Character> &chr,
    unsigned int product
) {
    check_turn(chr);
    check_town_location(chr);
    if (m_shops[chr->get_position()].count(product) == 0) {
        throw NoProductException();
    }
    remove_product_from_shop(chr->get_position(), product);
    m_remaining_products.push_back(product);
    end_trade(chr);
}

void Game::sell_product_in_special_cell(
    const std::shared_ptr<character::Character> &chr,
    unsigned int product
) {
    check_turn(chr);
    if (!chr->check_product(product)) {
        throw NoProductException();
    }
    if (m_all_products[product].get_place_of_cell() !=
        m_map.get_cell_map(chr->get_position()).get_special_type_cell()) {
        throw NoProductSaleException();
    }
    m_all_products[product].undo_product(chr);
    chr->erase_product(product);
    m_remaining_products.push_back(product);
    chr->change_gold(static_cast<int>(m_all_products[product].get_market_price()
    ));
}

void Game::start_new_round() {
    m_number_of_rounds += 1;
    if (m_number_of_rounds % 6 == 0) {
        for (int row = 0; row < m_map.get_size(); ++row) {
            for (int column = 0; column < m_map.get_size(); ++column) {
                if (m_map.get_cell_map(Point(row, column)).get_token() !=
                        AdventureType::NOTHING &&
                    m_map.get_cell_map(Point(row, column)).get_side_token() ==
                        Side::BACK) {
                    m_map.reverse_token(Point(row, column));
                }
            }
        }
    }
    if (m_number_of_rounds == 12) {
        m_boss_position = Point(11, 6);
        m_map.make_boss(m_boss_position);
    }
    if (m_number_of_rounds >= 25) {
        m_map.delete_boss(m_boss_position);
        m_boss_position.x -= 1;
        m_map.make_boss(m_boss_position);
        if (m_map.get_cell_map(m_boss_position).get_territory_name() ==
            "Talamir") {
            m_game_over = true;
        }
    }
}

void Game::call_to_fight(
    const std::shared_ptr<character::Character> &caller,
    const std::shared_ptr<character::Character> &receiver
) {
    check_turn(caller);
    check_sufficiency_action_points(1);
    receiver->call_to_fight(caller);
    caller->update_action_points(-1);
}

void Game::accept_to_fight(const std::shared_ptr<character::Character> &receiver
) {
    if (!receiver->check_caller_to_fight()) {
        throw NotCalledToFight();
    }
    auto caller = receiver->get_current_caller_to_fight();
    std::shared_ptr<fight::FightTwoPlayer> fight =
        std::make_shared<fight::FightTwoPlayer>(caller, receiver);
    caller->start_fight_two_player(fight, character::StateCharacter::CALLER);
    receiver->start_fight_two_player(
        fight, character::StateCharacter::RECEIVER
    );
    m_current_fight_two_player = caller->get_current_fight_two_player();
}

void Game::end_fight_two_player(const std::shared_ptr<character::Character> &chr
) {
    auto fight = chr->get_current_fight_two_player();
    fight->get_caller()->end_fight_two_player();
    fight->get_receiver()->end_fight_two_player();
    m_current_fight_two_player = nullptr;
}

std::vector <Point> Game::get_possible_moves() const {
    if (m_characters.empty()) {
        return {};
    }
    if (m_last_dice_movement_result.empty()) {
        if (m_characters[m_turn]->get_action_points() == 0) {
            return {};
        }
        return m_map.get_neighbours(m_characters[m_turn]->get_position());
    }
    return m_map.get_possible_moves(m_characters[m_turn]->get_position(), m_last_dice_movement_result);
}
}  // namespace game
}  // namespace runebound
