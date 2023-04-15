#include "game.hpp"
#include <nlohmann/json.hpp>
#include "point.hpp"
#include "runebound_fwd.hpp"
#include "card_fight.hpp"
#include <filesystem>

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

void Game::generate_all_cards_research() {
    std::vector<cards::CardResearch> cards;
    m_card_deck_research.resize(DECK_SIZE);
    for (int i = 0; i < DECK_SIZE; ++i) {
        m_all_cards_research.emplace_back(cards::CardResearch());
        m_card_deck_research[i] = i;
    }
}

void Game::generate_all_cards_fight() {
    std::vector<cards::CardResearch> cards;
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

void Game::relax(std::shared_ptr<character::Character> chr) {
    check_turn(chr);
    check_sufficiency_action_points(1);
    m_characters[m_turn]->relax();
    m_characters[m_turn]->update_action_points(-1);
}

void Game::check_and_get_card_adventure_because_of_token(
    std::shared_ptr<character::Character> chr
) {
    if (m_map.get_cell_map(chr->get_position()).get_token() !=
            ::runebound::AdventureType::NOTHING &&
        m_map.get_cell_map(chr->get_position()).get_side_token() ==
            ::runebound::Side::FRONT) {
        if (m_map.get_cell_map(chr->get_position()).get_token() ==
            ::runebound::AdventureType::RESEARCH) {
            unsigned int card =
                m_card_deck_research[rng() % m_card_deck_research.size()];
            chr->add_card(AdventureType::RESEARCH, card);
            pop_element_from_vector(card, m_card_deck_research);
        }
        m_map.get_cell_map(chr->get_position()).reverse_token();
    }
}

void Game::reverse_token(std::shared_ptr<character::Character> chr) {
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
        chr->start_fight(std::make_shared<::runebound::fight::Fight>(
            ::runebound::fight::Fight(
                chr, ::runebound::fight::Enemy(5, "Standard")
            )
        ));
    }
    m_map.reverse_token(position);
    m_characters[m_turn]->update_action_points(-2);
}

std::vector<Point> Game::make_move(
    const std::shared_ptr<character::Character> &chr,
    const Point &end,
    std::vector<::runebound::dice::HandDice> &dice_roll_results
) {
    check_turn(chr);
    check_sufficiency_action_points(2);
    std::vector<Point> result = m_map.check_move(
        m_characters[m_turn]->get_position(), end, dice_roll_results
    );
    if (result.empty()) {
        throw InaccessibleMoveException();
    }
    m_characters[m_turn]->set_position(end);
    m_characters[m_turn]->update_action_points(-2);
    return result;
}

}  // namespace game
}  // namespace runebound
