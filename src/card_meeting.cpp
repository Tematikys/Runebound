#include "card_meeting.hpp"
#include "nlohmann/json.hpp"

namespace runebound::cards {
void to_json(nlohmann::json &json, const CardMeeting &card) {
    json["m_card_type"] = card.m_card_type;
    json["m_card_name"] = card.m_card_name;
    json["first_option"] = card.m_first_option;
    json["second_option"] = card.m_second_option;
    json["m_trophy"] = card.m_trophy;
}

void from_json(const nlohmann::json &json, CardMeeting &card) {
    card.m_card_name = json["m_card_name"];
    from_json(json["m_first_option"], card.m_first_option);
    from_json(json["m_second_option"], card.m_second_option);
    card.m_trophy = json["m_trophy"];
    card.m_card_type = json["m_card_type"];
}
}  // namespace runebound::cards