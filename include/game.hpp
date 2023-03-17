#ifndef GAME_HPP_
#define GAME_HPP_

#include <json_fwd.hpp>
#include <map>
#include <memory>
#include <utility>
#include <vector>
#include "card_research.hpp"
#include "character.hpp"
#include "map.hpp"
#include "runebound_fwd.hpp"
#include "tokens.hpp"

namespace runebound {
const int DECK_SIZE = 60;

namespace game {
void to_json(nlohmann::json &json, const Game &game);
void from_json(const nlohmann::json &json, Game &game);

struct Game {
public:
    ::runebound::map::Map m_map;
    std::vector<::runebound::character::Character> m_characters;
    std::vector<unsigned int> m_card_deck;
    std::map<::runebound::token::Token, unsigned int> m_tokens;
    unsigned int m_turn = 0;
    const unsigned int M_COUNT_PLAYERS;
    const std::vector<cards::CardAdventure *> ALL_CARDS;

    static std::vector<cards::CardAdventure *> generate_all_cards();

public:
    Game() : M_COUNT_PLAYERS(1), ALL_CARDS(std::move(generate_all_cards())) {
        m_card_deck.resize(DECK_SIZE);
    };

    explicit Game(unsigned int count_players) : M_COUNT_PLAYERS(count_players) {
        m_characters.resize(count_players);
        m_card_deck.resize(DECK_SIZE);
        for (int i = 0; i < DECK_SIZE; ++i) {
            m_card_deck[i] = i;
        }
    }

    Game &operator=(const Game &other) {
        m_map = other.m_map;
        m_characters = other.m_characters;
        m_card_deck = other.m_card_deck;
        m_tokens = other.m_tokens;
        m_turn = other.m_turn;
        return *this;
    }

    Game(unsigned int count_players, std::vector<cards::CardAdventure *> cards)
        : M_COUNT_PLAYERS(count_players), ALL_CARDS(std::move(cards)) {
    }

    [[nodiscard]] int get_map_height() const {
        return m_map.get_height();
    }

    [[nodiscard]] int get_map_width() const {
        return m_map.get_width();
    }

    [[nodiscard]] int get_turn() const {
        return m_turn;
    }

    void make_move(
        const ::runebound::character::Character *chr,
        int end_x,
        int end_y,
        std::vector<::runebound::dice::HandDice> &dice_roll_results
    );

    friend void to_json(nlohmann::json &json, const Game &game);
    friend void from_json(const nlohmann::json &json, Game &game);
};
}  // namespace game
}  // namespace runebound
#endif  // GAME_HPP_
