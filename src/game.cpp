#include "game.hpp"
#include <chrono>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <thread>
#include "card_fight.hpp"
#include "point.hpp"
#include "product.hpp"
#include "runebound_fwd.hpp"
#include "skill_card.hpp"

namespace runebound {
namespace game {

void to_json(nlohmann::json &json, const Game &game) {
    json["m_game_over"] = game.m_game_over;
    json["m_map"] = game.m_map;
    json["m_characters"] = std::vector<character::Character>{};
    for (const auto &character : game.m_characters) {
        json["m_characters"].push_back(*character);
    }
    json["m_card_deck_research"] = game.m_card_deck_research;
    json["m_card_deck_fight"] = game.m_card_deck_fight;
    json["m_card_deck_skill"] = game.m_card_deck_skill;
    json["m_card_deck_meeting"] = game.m_card_deck_meeting;
    json["m_remaining_products"] = game.m_remaining_products;
    json["m_turn"] = game.m_turn;
    json["m_count_players"] = game.m_count_players;
    json["m_number_of_rounds"] = game.m_number_of_rounds;
    json["m_winner"] = game.m_winner;
    json["m_boss_position"] = game.m_boss_position;
    json["m_last_dice_movement_result"] = game.m_last_dice_movement_result;
    json["m_last_dice_relax_result"] = game.m_last_dice_relax_result;
    json["m_last_dice_research_result"] = game.m_last_dice_research_result;
    json["m_last_characteristic_check"] = game.m_last_characteristic_check;
    json["m_last_possible_outcomes"] = game.m_last_possible_outcomes;
    json["m_all_cards_research"] = game.m_all_cards_research;
    json["m_all_cards_fight"] = game.m_all_cards_fight;
    json["m_all_cards_meeting"] = game.m_all_cards_meeting;
    json["m_all_skill_cards"] = game.m_all_skill_cards;
    json["m_all_products"] = game.m_all_products;
    json["m_current_active_card_fight"] = game.m_current_active_card_fight;
    json["m_remaining_standard_characters"] =
        game.m_remaining_standard_characters;
    json["m_shops"] = game.m_shops;
    json["m_free_characters"] = game.m_free_characters;
    if (game.m_current_fight == nullptr) {
        json["m_current_fight"] = nullptr;
    } else {
        json["m_current_fight"] = *game.m_current_fight;
    }
    if (game.m_current_fight_two_player == nullptr) {
        json["m_current_fight_two_player"] = nullptr;
    } else {
        json["m_current_fight_two_player"] = *game.m_current_fight_two_player;
    }
}

namespace {
template <typename T>
void fill_vector(const nlohmann::json &json, std::vector<T> &vec) {
    vec.clear();
    for (const auto &elem : json) {
        vec.push_back(elem);
    }
}
}  // namespace

void from_json(const nlohmann::json &json, Game &game) {
    game.m_game_over = json["m_game_over"];
    game.m_map = json["m_map"];
    game.m_current_active_card_fight = json["m_current_active_card_fight"];
    game.m_characters.clear();
    for (const auto &character : json["m_characters"]) {
        game.m_characters.push_back(std::make_shared<character::Character>(
            character::Character(character)
        ));
    }
    fill_vector(json["m_card_deck_research"], game.m_card_deck_research);
    fill_vector(json["m_card_deck_fight"], game.m_card_deck_fight);
    fill_vector(json["m_card_deck_skill"], game.m_card_deck_skill);
    fill_vector(json["m_card_deck_meeting"], game.m_card_deck_meeting);
    fill_vector(json["m_remaining_products"], game.m_remaining_products);
    game.m_turn = json["m_turn"];
    game.m_count_players = json["m_count_players"];
    game.m_number_of_rounds = json["m_number_of_rounds"];
    game.m_winner = json["m_winner"];
    game.m_boss_position = json["m_boss_position"];
    fill_vector(
        json["m_last_dice_movement_result"], game.m_last_dice_movement_result
    );
    fill_vector(
        json["m_last_dice_relax_result"], game.m_last_dice_relax_result
    );
    fill_vector(
        json["m_last_dice_research_result"], game.m_last_dice_research_result
    );
    game.m_last_characteristic_check = json["m_last_characteristic_check"];
    fill_vector(
        json["m_last_possible_outcomes"], game.m_last_possible_outcomes
    );
    game.m_all_cards_research.clear();
    for (const auto &json_card_research : json["m_all_cards_research"]) {
        cards::CardResearch card;
        from_json(json_card_research, card, game.m_map);
        game.m_all_cards_research.push_back(card);
    }
    fill_vector(json["m_all_cards_fight"], game.m_all_cards_fight);
    fill_vector(json["m_all_cards_meeting"], game.m_all_cards_meeting);
    fill_vector(json["m_all_skill_cards"], game.m_all_skill_cards);
    fill_vector(json["m_all_products"], game.m_all_products);
    game.m_remaining_standard_characters =
        std::set<character::StandardCharacter>(
            json["m_remaining_standard_characters"].begin(),
            json["m_remaining_standard_characters"].end()
        );

    game.m_free_characters = std::set<character::StandardCharacter>(
        json["m_free_characters"].begin(), json["m_free_characters"].end()
    );
    game.m_shops = std::map<Point, std::set<unsigned int>>(
        json["m_shops"].begin(), json["m_shops"].end()
    );
    if (json["m_current_fight"] != nullptr) {
        fight::Fight fight;
        from_json(json["m_current_fight"], fight, game);
        game.m_current_fight =
            std::make_shared<fight::Fight>(fight::Fight(fight));
    } else {
        game.m_current_fight = nullptr;
    }

    if (json["m_current_fight_two_player"] != nullptr) {
        fight::FightTwoPlayer fight_two_player;
        from_json(json["m_current_fight_two_player"], fight_two_player, game);
        game.m_current_fight_two_player =
            std::make_shared<fight::FightTwoPlayer>(
                fight::FightTwoPlayer(fight_two_player)
            );
    } else {
        game.m_current_fight_two_player = nullptr;
    }
}

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
        if (m_all_skill_cards[card].check_success()) {
            return true;
        }
    }
    return false;
}

void Game::start_next_character_turn(
    const std::shared_ptr<character::Character> &chr
) {
    check_turn(chr);
    m_last_dice_movement_result.clear();
    m_last_dice_research_result.clear();
    m_last_dice_relax_result.clear();
    m_last_characteristic_check = false;
    m_turn = (m_turn + 1) % m_count_players;
    m_characters[m_turn]->restore_action_points();
    if (m_turn == 0) {
        start_new_round();
    }
}

void Game::add_bot() {
    if (m_remaining_standard_characters.empty()) {
        throw CharacterAlreadySelected();
    }
    auto standard_character = *m_remaining_standard_characters.begin();
    m_characters.emplace_back(
        std::make_shared<::runebound::character::Character>(
            ::runebound::character::Character(standard_character)
        )
    );
    m_characters.back()->make_new_state_in_game(
        character::StateCharacterInGame::BOT
    );
    m_count_players += 1;
    m_free_characters.insert(standard_character);
    m_remaining_standard_characters.erase(standard_character);
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
        m_current_active_card_fight = card;
        chr->add_card(AdventureType::FIGHT, card);
        m_card_deck_fight.erase(
            std::find(m_card_deck_fight.begin(), m_card_deck_fight.end(), card)
        );
        chr->start_fight(std::make_shared<fight::Fight>(
            chr, m_all_cards_fight[card].get_enemy()
        ));
        m_current_fight = chr->get_current_fight();

        m_characters[get_enemy(m_turn)]->start_fight_as_enemy();
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
        m_characters[get_enemy(m_turn)]->start_fight_as_enemy();
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
    m_characters[get_enemy(m_turn)]->end_fight_as_enemy();
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
    m_characters[get_enemy(m_turn)]->end_fight_as_enemy();
}

std::vector<Point> Game::make_move(
    const std::shared_ptr<character::Character> &chr,
    const Point &end,
    std::vector<::runebound::dice::HandDice> &dice_roll_results
) {
    check_turn(chr);
    check_characters_in_map_cell(end);
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
    m_last_characteristic_check = false;
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
        m_last_characteristic_check = true;
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

std::vector<Point> Game::get_possible_moves() const {
    if (m_characters.empty()) {
        return {};
    }
    auto result = m_map.get_possible_moves(
        m_characters[m_turn]->get_position(), m_last_dice_movement_result
    );
    for (const auto &character : m_characters) {
        if (result.count(character->get_position()) > 0) {
            result.erase(character->get_position());
        }
    }
    std::vector<Point> possible_moves;
    for (const auto &cell : result) {
        possible_moves.push_back(cell);
    }
    return possible_moves;
}

void Game::exit_game(const std::shared_ptr<character::Character> &chr) {
    if (m_free_characters.count(chr->get_standard_character()) > 0) {
        throw NotSelectedCharacter();
    }
    chr->make_new_state_in_game(character::StateCharacterInGame::INACTIVE);
    m_free_characters.insert(chr->get_standard_character());
}

void Game::exit_game_and_replace_with_bot(
    const std::shared_ptr<character::Character> &chr
) {
    exit_game(chr);
    chr->make_new_state_in_game(character::StateCharacterInGame::BOT);
}

void Game::join_game(character::StandardCharacter character) {
    if (m_free_characters.count(character) == 0) {
        throw NotSelectedCharacter();
    }
    m_free_characters.erase(character);
    get_character_by_standard_characters(character)->make_new_state_in_game(
        character::StateCharacterInGame::PLAYER
    );
}

}  // namespace game
}  // namespace runebound
