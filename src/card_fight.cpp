#include "card_fight.hpp"
#include "fight.hpp"

namespace runebound::cards {
void to_json(nlohmann::json &json, const CardFight &card) {
    json["m_card_type"] = card.m_card_type;
    json["m_trophy"] = card.m_trophy;
    json["m_gold_award"] = card.m_gold_award;
    json["m_enemy"] = card.m_enemy;
}

void from_json(const nlohmann::json &json, CardFight &card) {
    ::runebound::fight::from_json(json["m_enemy"], card.m_enemy);
    card.m_gold_award = json["m_gold_award"];
    card.m_trophy = json["m_trophy"];
    card.m_card_type = json["m_card_type"];
}
} // namespace runebound::cards