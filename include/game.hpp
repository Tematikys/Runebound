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
private:
    ::runebound::map::Map m_map;
    std::vector<::runebound::character::Character> m_characters;
    std::vector<unsigned int> m_card_deck;
    std::map<::runebound::token::Token, unsigned int> m_tokens;
    unsigned int m_turn = 0;
    unsigned int m_count_players;
    std::vector<unsigned int> m_indexes_card_research;
    const std::vector<cards::CardAdventure *> ALL_CARDS;

    std::vector<cards::CardAdventure *> generate_all_cards();

    template <typename T>
    void pop_element_from_vector(T element, std::vector<T> &vec) {
        for (std::size_t i = 0; i < vec.size(); ++i) {
            if (vec[i] == element) {
                std::swap(vec[i], vec.back());
                break;
            }
        }
        vec.pop_back();
    }

public:
    Game() : ALL_CARDS(std::move(generate_all_cards())) {
        m_card_deck.resize(DECK_SIZE);
        for (int i = 0; i < DECK_SIZE; ++i) {
            m_card_deck[i] = i;
        }
    };


    Game &operator=(const Game &other) {
        m_map = other.m_map;
        m_characters = other.m_characters;
        m_card_deck = other.m_card_deck;
        m_tokens = other.m_tokens;
        m_turn = other.m_turn;
        return *this;
    }

    Game(std::vector<cards::CardAdventure *> cards)
        : ALL_CARDS(std::move(cards)) {
    }

    ::runebound::character::Character *make_character(
        int gold,
        int health,
        int current_x,
        int current_y,
        unsigned int hand_limit,
        unsigned int speed,
        std::string name
    ) {
        m_characters.emplace_back(::runebound::character::Character(
            gold, health, current_x, current_y, hand_limit, speed, name
        ));
        m_count_players += 1;
        return &m_characters.back();
    }

    [[nodiscard]] int get_map_height() const {
        return m_map.get_height();
    }

    [[nodiscard]] std::pair<int, int> get_position_character(
        ::runebound::character::Character *chr
    ) const;

    [[nodiscard]] int get_map_width() const {
        return m_map.get_width();
    }

    [[nodiscard]] unsigned int get_turn() const {
        return m_turn;
    }

    void check_and_get_card_adventure_because_of_token(
        ::runebound::character::Character *chr
    );

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
