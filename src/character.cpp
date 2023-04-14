#include "character.hpp"
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include "fight_token.hpp"

namespace runebound {
namespace character {

void Character::load_character_from_file(const std::string &file) {
    std::ifstream in(file);
    nlohmann::json json_character;
    in >> json_character;
    ::runebound::character::from_json(json_character, *this);
}

Character::Character(const StandardCharacter &chr) {
    switch (chr) {
        case (StandardCharacter::LISSA): {
            load_character_from_file("../data/json/lissa.json");
            break;
        }
        case (StandardCharacter::MASTER_THORN): {
            load_character_from_file("../data/json/master_thorn.json");
            break;
        }
        case (StandardCharacter::CORBAN): {
            load_character_from_file("../data/json/corban.json");
            break;
        }
        case (StandardCharacter::LORD_HAWTHORNE): {
            load_character_from_file("../data/json/lord_hawthorne.json");
            break;
        }
        case (StandardCharacter::LAUREL_FROM_BLOODWOOD): {
            load_character_from_file("../data/json/laurel_from_bloodwood.json");
            break;
        }
        case (StandardCharacter::ELDER_MOK): {
            load_character_from_file("../data/json/elder_mok.json");
            break;
        }
    }
}

void Character::pop_card(unsigned int card) {
    for (std::size_t i = 0; i < m_cards.size(); ++i) {
        if (m_cards[i] == card) {
            std::swap(m_cards[i], m_cards.back());
            break;
        }
    }
    m_cards.pop_back();
}

void to_json(nlohmann::json &json, const Character &character) {
    json["m_hand_limit"] = character.m_hand_limit;
    json["m_speed"] = character.m_speed;
    json["m_action_points"] = character.m_action_points;
    json["m_max_action_points"] = character.m_max_action_points;
    json["m_name"] = character.m_name;
    json["m_gold"] = character.m_gold;
    json["m_health"] = character.m_health;
    json["m_max_health"] = character.m_max_health;
    json["m_current_position"] = character.m_current_position;
    json["m_tokens"] = character.m_tokens;
    json["m_cards"] = character.m_cards;
    json["m_fight_tokens"] = character.m_fight_tokens;
}

void from_json(const nlohmann::json &json, Character &character) {
    character.m_hand_limit = json["m_hand_limit"];
    character.m_speed = json["m_speed"];
    character.m_action_points = json["m_action_points"];
    character.m_max_action_points = json["m_max_action_points"];
    character.m_name = json["m_name"];
    character.m_gold = json["m_gold"];
    character.m_health = json["m_health"];
    character.m_max_health = json["m_max_health"];
    character.m_current_position = json["m_current_position"];
    character.m_tokens = json["m_tokens"];
    character.m_cards.clear();
    for (const auto &fight_token : json["m_fight_tokens"]) {
        character.m_fight_tokens.push_back(fight_token);
    }
    for (const auto &card : json["m_cards"]) {
        character.m_cards.push_back(card);
    }
    character.m_fight_tokens.clear();
}
}  // namespace character
}  // namespace runebound
