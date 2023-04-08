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
    HandFightTokens first;
    bool first_lead;
    HandFightTokens second;
    bool second_lead;

    FightToken(HandFightTokens first_, bool first_lead_, HandFightTokens second_, bool second_lead_) :
    first(first_), first_lead(first_lead_), second(second_), second_lead(second_lead_) {}

    bool operator==(const FightToken &token) const {
        return first_lead == token.first_lead &&
               second_lead == token.second_lead && first == token.first &&
               second == token.second;
    }
};

}  // namespace runebound::fight

#endif  // FIGHT_TOKEN_HPP_
