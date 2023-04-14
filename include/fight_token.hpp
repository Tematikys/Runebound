#ifndef FIGHT_TOKEN_HPP_
#define FIGHT_TOKEN_HPP_

#include "nlohmann/json.hpp"

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

void to_json(nlohmann::json &json, const FightToken &fight_token);
void from_json(const nlohmann::json &json, FightToken &fight_token);

struct FightToken {
public:
    HandFightTokens first = HandFightTokens::NOTHING;
    bool first_lead = 0;
    int first_count = 0;
    HandFightTokens second = HandFightTokens::NOTHING;
    bool second_lead = 0;
    int second_count = 0;

    FightToken() = default;

    FightToken(
        HandFightTokens first_,
        bool first_lead_,
        int first_count_,
        HandFightTokens second_,
        bool second_lead_,
        int second_count_
    )
        : first(first_),
          first_lead(first_lead_),
          first_count(first_count_),
          second(second_),
          second_lead(second_lead_),
          second_count(second_count_) {
    }

    bool operator==(const FightToken &token) const {
        return first_lead == token.first_lead &&
               second_lead == token.second_lead && first == token.first &&
               second == token.second && first_count == token.first_count &&
               second_count == token.second_count;
    }

    friend void to_json(nlohmann::json &json, const FightToken &fight_token) {
        json["first"] = fight_token.first;
        json["first_lead"] = fight_token.first_lead;
        json["first_count"] = fight_token.first_count;
        json["second"] = fight_token.second;
        json["second_lead"] = fight_token.second_lead;
        json["second_count"] = fight_token.second_count;
    }

    friend void from_json(const nlohmann::json &json, FightToken &fight_token) {
        fight_token.first = json["first"];
        fight_token.second = json["second"];
        fight_token.first_lead = json["first_lead"];
        fight_token.second_lead = json["second_lead"];
        fight_token.first_count = json["first_count"];
        fight_token.second_count = json["second_count"];
    }
};

}  // namespace runebound::fight

#endif  // FIGHT_TOKEN_HPP_
