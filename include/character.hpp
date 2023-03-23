#ifndef CHARACTER_HPP_
#define CHARACTER_HPP_

#include <json_fwd.hpp>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include "card_adventure.hpp"
#include "point.hpp"
#include "runebound_fwd.hpp"
#include "tokens.hpp"

namespace runebound {
namespace character {

void to_json(nlohmann::json &json, const Character &character);
void from_json(const nlohmann::json &json, Character &character);

struct Character {
private:
    unsigned int m_hand_limit, m_speed;
    unsigned int m_action_points = 3;
    std::string m_name;
    std::vector<unsigned int> m_cards;
    std::map<runebound::token::Token, int> m_tokens;
    int m_max_health;

public:
    int m_gold, m_health;
    Point m_current_position;

    Character()
        : m_gold(0),
          m_health(0),
          m_hand_limit(0),
          m_speed(0),
          m_max_health(0),
          m_current_position(0, 0) {
    }

    explicit Character(
        int gold,
        int health,
        Point current_position,
        unsigned int hand_limit,
        unsigned int speed,
        std::string name
    )
        : m_gold(gold),
          m_health(health),
          m_max_health(health),
          m_current_position(current_position),
          m_hand_limit(hand_limit),
          m_speed(speed),
          m_name(std::move(name)) {
    }

    [[nodiscard]] std::string get_name() const {
        return m_name;
    }

    void add_action_points(unsigned int delta) {
        m_action_points += delta;
    }

    unsigned int get_speed() const {
        return m_speed;
    }

    [[nodiscard]] unsigned int get_action_points() const {
        return m_action_points;
    }

    void add_card(unsigned int card) {
        m_cards.push_back(card);
    }

    void pop_card(unsigned int card);

    friend void to_json(nlohmann::json &json, const Character &character);
    friend void from_json(const nlohmann::json &json, Character &character);

    nlohmann::json to_json() {
        nlohmann::json json;
        ::runebound::character::to_json(json, *this);
        return json;
    }

    static Character from_json(const nlohmann::json &json) {
        Character chr;
        ::runebound::character::from_json(json, chr);
        return chr;
    }
};
}  // namespace character
}  // namespace runebound
#endif  // CHARACTER_HPP_
