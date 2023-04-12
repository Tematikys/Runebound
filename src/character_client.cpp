#include "character_client.hpp"
#include <nlohmann/json.hpp>
#include "tokens.hpp"

namespace runebound::character {

void to_json(nlohmann::json &json, const CharacterClient &character) {
    json["m_hand_limit"] = character.m_hand_limit;
    json["m_current_action_points"] = character.m_current_action_points;
    json["m_max_action_points"] = character.m_max_action_points;
    json["m_max_health"] = character.m_max_health;
    json["m_current_health"] = character.m_current_health;
    json["m_name"] = character.m_name;
    json["m_cards"] = character.m_cards;
    json["m_tokens"] = character.m_tokens;
}

void from_json(const nlohmann::json &json, CharacterClient &character) {
    character.m_hand_limit = json["m_hand_limit"];
    character.m_current_action_points = json["m_current_action_points"];
    character.m_max_action_points = json["m_max_action_points"];
    character.m_max_health = json["m_max_health"];
    character.m_current_health = json["m_current_health"];
    character.m_name = json["m_name"];
    character.m_tokens = json["m_tokens"];
    character.m_cards.clear();
    for (const auto &card : json["m_cards"]) {
        character.m_cards.push_back(card);
    }
}

}  // namespace runebound::character