#ifndef CHARACTER_HPP_
#define CHARACTER_HPP_

#include <memory>
#include <set>
#include <string>
#include <vector>
#include "../src/card_adventure.hpp"
#include "runebound_fwd.hpp"

namespace runebound {
namespace character {
gt
struct Character {
private:
    int m_gold, m_health;
    unsigned int m_hand_limit, m_speed;
    std::string m_name;
    std::vector<std::unique_ptr<::runebound::cards::CardAdventure>> m_cards;
    std::set <int> m_tokens; // TODO

public:
    Character() : m_gold(0), m_health(0), m_hand_limit(0), m_speed(0) {}

    Character(int gold, int health, unsigned int hand_limit, unsigned int speed, const std::string &name)
        : m_gold(gold), m_health(health), m_hand_limit(hand_limit), m_speed(speed), m_name(name) {
    }

    void add_gold(int value) {
        m_gold += value;
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

    std::unique_ptr <::runebound::cards::CardAdventure> pop_card(::runebound::cards::CardAdventure *card);
};
} // namespace character
} // namespace runebound
#endif  // CHARACTER_HPP_
