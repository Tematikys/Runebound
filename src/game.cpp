#include "game.hpp"
#include <json.hpp>
#include "runebound_fwd.hpp"
#include "point.hpp"

namespace runebound {
namespace game {

void to_json(nlohmann::json &json, const Game &game) {
    json["m_map"] = game.m_map;
    json["m_characters"] = game.m_characters;
    json["m_card_deck"] = game.m_card_deck;
    json["m_tokens"] = game.m_tokens;
    json["m_turn"] = game.m_turn;
    json["m_count_players"] = game.m_count_players;
    std::vector<nlohmann::json> cards(game.ALL_CARDS.size());
    for (std::size_t card = 0; card < game.ALL_CARDS.size(); ++card) {
        cards[card] = game.ALL_CARDS[card]->to_json();
    }
    json["ALL_CARDS"] = cards;
}

void from_json(const nlohmann::json &json, Game &game) {
    int count_players = json["M_COUNT_PLAYERS"];
    std::vector<cards::CardAdventure *> cards;
    for (const nlohmann::json &card : json["ALL_CARDS"]) {
        ::runebound::AdventureType type = card["m_card_type"];
        switch (type) {
            case ::runebound::AdventureType::RESEARCH: {
                cards.push_back(new cards::CardResearch());
                from_json(
                    card, dynamic_cast<::runebound::cards::CardResearch *>(
                              cards.back()
                          )
                );
                break;
            }
        }
    }
    game = Game(cards);
    game.m_count_players = json["m_count_players"];
    game.m_map = json["m_map"];
    game.m_characters = json["m_characters"];
    game.m_card_deck.clear();
    for (const auto &card : json["m_card_deck"]) {
        game.m_card_deck.push_back(card);
    }
    game.m_tokens = json["m_tokens"];
    game.m_turn = json["m_turn"];
}

Point Game::get_position_character(
    ::runebound::character::Character *chr
) const {
    return chr->m_current_position;
}

std::vector<cards::CardAdventure *> Game::generate_all_cards() {
    std::vector<cards::CardAdventure *> cards;
    m_indexes_card_research.resize(DECK_SIZE);
    for (int i = 0; i < DECK_SIZE; ++i) {
        cards.push_back(new cards::CardResearch());
        m_indexes_card_research[i] = i;
    }
    return cards;
}

void Game::check_and_get_card_adventure_because_of_token(
    ::runebound::character::Character *chr
) {
    if (m_map.get_cell_map(chr->m_current_position).get_token() !=
            ::runebound::AdventureType::NOTHING &&
        m_map.get_cell_map(chr->m_current_position)
                .get_side_token() == ::runebound::Side::FRONT) {
        if (m_map.get_cell_map(chr->m_current_position)
                .get_token() == ::runebound::AdventureType::RESEARCH) {
            unsigned int card =
                m_indexes_card_research[rng() % m_indexes_card_research.size()];
            chr->add_card(card);
            pop_element_from_vector(card, m_card_deck);
            pop_element_from_vector(card, m_indexes_card_research);
        }
        m_map.get_cell_map(chr->m_current_position).reverse_token();
    }
}

void Game::make_move(
    const ::runebound::character::Character *chr,
    const Point &end,
    std::vector<::runebound::dice::HandDice> &dice_roll_results
) {
    if (chr != &m_characters[m_turn]) {
        return;
    }
    if (!m_map.check_move(
            m_characters[m_turn].m_current_position,
            end, dice_roll_results
        )) {
        return;
    }
    m_characters[m_turn].m_current_position = end;
    m_turn = (m_turn + 1) % m_count_players;
}

}  // namespace game
}  // namespace runebound
