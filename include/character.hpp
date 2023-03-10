#ifndef CHARACTER_HPP_
#define CHARACTER_HPP_

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>
#include "card_adventure.hpp"
#include "runebound_fwd.hpp"
#include "tokens.hpp"

namespace runebound {
namespace character {

void to_json(nlohmann::json &json, const Character& character);
void from_json(const nlohmann::json &json, Character& character);

struct Character {
private:
    unsigned int m_hand_limit, m_speed;
    std::string m_name;
    std::vector<std::unique_ptr<::runebound::cards::CardAdventure>> m_cards;
    std::map<runebound::token::Token, int> m_tokens;

public:
    int m_gold, m_health, m_current_x, m_current_y;

    Character()
        : m_gold(0),
          m_health(0),
          m_hand_limit(0),
          m_speed(0),
          m_current_x(0),
          m_current_y(0) {
    }

    Character(
        int gold,
        int health,
        int current_x,
        int current_y,
        unsigned int hand_limit,
        unsigned int speed,
        std::string name
    )
        : m_gold(gold),
          m_health(health),
          m_current_x(current_x),
          m_current_y(current_y),
          m_hand_limit(hand_limit),
          m_speed(speed),
          m_name(std::move(name)) {
    }



    [[nodiscard]] std::string get_name() const {
        return m_name;
    }

    void add_card(std::unique_ptr<::runebound::cards::CardAdventure> card) {
        m_cards.push_back(std::move(card));
    }

    std::unique_ptr<::runebound::cards::CardAdventure> pop_card(
        ::runebound::cards::CardAdventure *card
    );

    friend void to_json(nlohmann::json &json, const Character& character);
    friend void from_json(const nlohmann::json &json, Character& character);
};
}  // namespace character
}  // namespace runebound
#endif  // CHARACTER_HPP_
