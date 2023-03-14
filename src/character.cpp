#include "character.hpp"
#include <json.hpp>
#include <map>
#include <memory>

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
    std::vector <nlohmann::json> json_cards;
    for (const auto &card : character.m_cards) {
        json_cards.push_back(card->to_json());
    }
    json["m_cards"].clear();
}

void from_json(const nlohmann::json &json, Character& character) {
    character.m_hand_limit = json["m_hand_limit"];
    character.m_speed = json["m_speed"];
    character.m_name = json["m_name"];
    character.m_gold = json["m_gold"];
    character.m_health = json["m_health"];
    character.m_current_x = json["m_current_x"];
    character.m_current_y = json["m_current_y"];
    character.m_tokens = json["m_tokens"];
    character.m_cards.clear();
    for (auto &card : json["m_cards"]) {
        cards::CardAdventure &card_ref;
        from_json(card, card_ref);
        character.m_cards.push_back(std::make_unique<cards::CardAdventure>(card_ref));
    }
}
}  // namespace character
}  // namespace runebound
