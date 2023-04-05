#ifndef FIGHT_HPP_
#define FIGHT_HPP_

#include <memory>
#include <vector>
#include "character.hpp"
#include "runebound_fwd.hpp"

namespace runebound::fight {
enum class HandFightTokens {
    PHYSICAL_DAMAGE,
    MAGICAL_DAMAGE,
    DEXTERITY,
    HIT,
    ENEMY_DAMAGE,
    DOUBLING,
    SHIELD,
    NOTHING
};

struct FightToken {
public:
    const bool first_lead;
    const bool second_lead;
    const HandFightTokens first;
    const HandFightTokens second;
};

struct Enemy {
private:
    int m_health;
    const std::string m_name;
    std::vector<FightToken> m_fight_tokens;

public:
    void update_health(int delta) {
        m_health += delta;
    }

    int get_health() const {
        return m_health;
    }
};

struct Fight {
private:
    friend struct Character;
    friend struct Enemy;
    std::shared_ptr<::runebound::character::Character> m_character;
    ::runebound::fight::Enemy m_enemy;

    void update_state_character(int delta_health) {
        m_character->update_health(delta_health);
    }

    void update_state_enemy(int delta_health) {
        m_enemy.update_health(delta_health);
    }

public:
};
}  // namespace runebound::fight

#endif  // FIGHT_HPP_
