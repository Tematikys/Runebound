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

struct Character {
private:
    int m_gold, m_health;
    unsigned int m_hand_limit, m_speed;
    std::string m_name;
    int m_current_x, m_current_y;
    std::vector<std::unique_ptr<::runebound::cards::CardAdventure>> m_cards;
    std::map<runebound::token::Token, int> m_tokens;

public:
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
        const std::string &name
    )
        : m_gold(gold),
          m_health(health),
          m_current_x(current_x),
          m_current_y(current_y),
          m_hand_limit(hand_limit),
          m_speed(speed),
          m_name(name) {
    }

    void add_gold(int value) {
        m_gold += value;
    }

    int get_current_x() const {
        return m_current_x;
    }

    int get_current_y() const {
        return m_current_y;
    }

    void set_current_x(int x) {
        m_current_x = x;
    }

    void set_current_y(int y) {
        m_current_y = y;
    }

    [[nodiscard]] int get_gold() const {
        return m_gold;
    }

    std::string get_name() const {
        return m_name;
    }

    void add_injury(int injury) {
        m_health -= injury;
    }

    void add_health(int health) {
        m_health += health;
    }

    void add_card(std::unique_ptr<::runebound::cards::CardAdventure> card) {
        m_cards.push_back(std::move(card));
    }

    std::unique_ptr<::runebound::cards::CardAdventure> pop_card(
        ::runebound::cards::CardAdventure *card
    );
};
}  // namespace character
}  // namespace runebound
#endif  // CHARACTER_HPP_
