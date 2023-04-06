#ifndef FIGHT_TOKEN_HPP_
#define FIGHT_TOKEN_HPP_

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
    bool first_lead;
    bool second_lead;
    HandFightTokens first;
    HandFightTokens second;

    bool operator==(const FightToken &token) const {
        return first_lead == token.first_lead &&
               second_lead == token.second_lead && first == token.first &&
               second == token.second;
    }
};

}  // namespace runebound::fight

#endif  // FIGHT_TOKEN_HPP_
