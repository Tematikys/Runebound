#ifndef GAME_HPP_
#define GAME_HPP_

#include <map>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <utility>
#include <vector>
#include "card_research.hpp"
#include "character.hpp"
#include "map.hpp"
#include "runebound_fwd.hpp"
#include "tokens.hpp"
#include <utility>
#include <memory>

namespace runebound {
const int DECK_SIZE = 60;

namespace game {
void to_json(nlohmann::json &json, const Game &game);
void from_json(const nlohmann::json &json, Game &game);

struct Game {

    ::runebound::map::Map m_map;
private:

    std::vector<::runebound::character::Character> m_characters;
    std::vector<unsigned int> m_card_deck_research;
    std::map<::runebound::token::Token, unsigned int> m_tokens;
    unsigned int m_turn = 0;
    unsigned int m_count_players;
    std::vector<unsigned int> m_indexes_card_research;
    const std::vector<cards::CardResearch> ALL_CARDS_RESEARCH;

    std::vector<cards::CardResearch> generate_all_cards_research();

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
    Game() : ALL_CARDS_RESEARCH(std::move(generate_all_cards_research())) {
        m_card_deck_research.resize(DECK_SIZE);
        for (int i = 0; i < DECK_SIZE; ++i) {
            m_card_deck_research[i] = i;
        }
    };

    Game &operator=(const Game &other) {
        m_map = other.m_map;
        m_characters = other.m_characters;
        m_card_deck_research = other.m_card_deck_research;
        m_tokens = other.m_tokens;
        m_turn = other.m_turn;
        return *this;
    }

    std::shared_ptr<::runebound::character::Character> make_character(
        int gold,
        int health,
        const Point &current,
        unsigned int hand_limit,
        unsigned int speed,
        std::string name,
        const std::vector <::runebound::fight::FightToken> tokens
    ) {
        m_characters.emplace_back(::runebound::character::Character(
            gold, health, current, hand_limit, speed, name, tokens
        ));
        m_count_players += 1;
        return std::make_shared<character::Character>(m_characters.back());
    }

    [[nodiscard]] int get_map_size() const {
        return m_map.get_size();
    }

    void reverse_token(int row, int column) {
        m_map.reverse_token(Point(row, column));
    }

    [[nodiscard]] Point get_position_character(
        ::runebound::character::Character *chr
    ) const;

    [[nodiscard]] unsigned int get_turn() const {
        return m_turn;
    }

    void check_and_get_card_adventure_because_of_token(
        ::runebound::character::Character *chr
    );

    std::vector<::runebound::dice::HandDice> throw_camping_dice(
        const ::runebound::character::Character *chr
    ) const {
        return ::runebound::dice::get_combination_of_dice(chr->get_speed());
    }

    std::vector<Point> make_move(
        const ::runebound::character::Character *chr,
        const Point &point,
        std::vector<::runebound::dice::HandDice> &dice_roll_results
    );

    friend void to_json(nlohmann::json &json, const Game &game);
    friend void from_json(const nlohmann::json &json, Game &game);

    [[nodiscard]] nlohmann::json to_json() const {
        nlohmann::json json;
        ::runebound::game::to_json(json, *this);
        return json;
    }

    static Game from_json(const nlohmann::json &json) {
        Game game;
        ::runebound::game::from_json(json, game);
        return game;
    }
};
}  // namespace game
}  // namespace runebound
#endif  // GAME_HPP_
