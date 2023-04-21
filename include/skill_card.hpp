#ifndef SKILL_CARD_HPP_
#define SKILL_CARD_HPP_

#include "nlohmann/json.hpp"
#include "runebound_fwd.hpp"

namespace runebound::cards {

enum class Characteristic { BODY, INTELLIGENCE, SPIRIT };

void to_json(nlohmann::json &json, const SkillCard &card);
void from_json(const nlohmann::json &json, SkillCard &card);

struct SkillCard {
private:
    bool m_success;
    Characteristic m_characteristic;
    int m_required_number_trophies;

public:
    SkillCard(bool success, Characteristic characteristic, int trophies)
        : m_success(success), m_characteristic(characteristic),
          m_required_number_trophies(trophies) {
    }

    friend void to_json(nlohmann::json &json, const SkillCard &card) {
        json["m_success"] = card.m_success;
        json["m_characteristic"] = card.m_characteristic;
        json["m_required_number_trophies"] = card.m_characteristic;
    }

    friend void from_json(const nlohmann::json &json, SkillCard &card) {
        card.m_success = json["m_success"];
        card.m_characteristic = json["m_characteristic"];
        card.m_required_number_trophies = json["m_required_number_trophies"];
    }

};
} // namespace runebound::cards
#endif  // SKILL_CARD_HPP_

