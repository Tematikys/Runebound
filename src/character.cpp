#include "character.hpp"
#include "nlohmann/json.hpp"
#include <map>

namespace runebound {
namespace character {
std::unique_ptr<::runebound::cards::CardAdventure> Character::pop_card(
    ::runebound::cards::CardAdventure *card
) {
    for (std::size_t i = 0; i < m_cards.size(); ++i) {
        if (m_cards[i].get() == card) {
            std::swap(m_cards[i], m_cards.back());
            break;
        }
    }
    auto desired_card = std::move(m_cards.back());
    m_cards.pop_back();
    return desired_card;
}

void to_json(nlohmann::json &json, const Character& character) {
    json["m_hand_limit"] = character.m_hand_limit;
    json["m_speed"] = character.m_speed;
    json["m_name"] = character.m_name;
    json["m_gold"] = character.m_gold;
    json["m_health"] = character.m_health;
    json["m_current_x"] = character.m_current_x;
    json["m_current_y"] = character.m_current_y;
    json["m_tokens"] = character.m_tokens;
}

void from_json(const nlohmann::json &json, Character& character) {
    character.m_hand_limit = json["m_hand_limit"];
    character.m_speed = json["m_speed"];
    character.m_name = json["m_name"];
    character.m_gold = json["m_gold"];
    character.m_health = json["m_health"];
    character.m_current_x = json["m_current_x"];
    character.m_current_y = json["m_current_y"];
    for (const auto &token : json["m_tokens"]) {
        character.m_tokens.emplace(token);
    }
}
}  // namespace character
}  // namespace runebound
