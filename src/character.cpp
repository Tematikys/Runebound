#include "character.hpp"
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include "card_fight.hpp"
#include "fight.hpp"
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
            load_character_from_file("data/json/characters/lissa.json");
            break;
        }
        case (StandardCharacter::MASTER_THORN): {
            load_character_from_file("data/json/characters/master_thorn.json");
            break;
        }
        case (StandardCharacter::CORBIN): {
            load_character_from_file("data/json/characters/corbin.json");
            break;
        }
        case (StandardCharacter::LORD_HAWTHORNE): {
            load_character_from_file("data/json/characters/lord_hawthorne.json"
            );
            break;
        }
        case (StandardCharacter::LAUREL_FROM_BLOODWOOD): {
            load_character_from_file(
                "data/json/characters/laurel_from_bloodwood.json"
            );
            break;
        }
        case (StandardCharacter::ELDER_MOK): {
            load_character_from_file("data/json/characters/elder_mok.json");
            break;
        }
    }
    m_standard_character = chr;
}

void Character::add_card(AdventureType type, unsigned int card) {
    if (type == AdventureType::FIGHT) {
        m_cards_fight.insert(card);
    }
    if (type == AdventureType::RESEARCH) {
        m_cards_research.insert(card);
    }
    if (type == AdventureType::MEETING) {
        m_cards_meeting.insert(card);
    }
}

bool Character::check_card(AdventureType type, unsigned int card) const {
    if (type == AdventureType::MEETING) {
        return m_cards_meeting.count(card) != 0;
    }
    if (type == AdventureType::FIGHT) {
        return m_cards_fight.count(card) != 0;
    }
    return m_cards_research.count(card) != 0;
}

void Character::pop_card(AdventureType type, unsigned int card) {
    if (type == AdventureType::RESEARCH) {
        m_cards_research.erase(card);
    }
    if (type == AdventureType::FIGHT) {
        m_cards_fight.erase(card);
    }
    if (type == AdventureType::MEETING) {
        m_cards_meeting.erase(card);
    }
}

void Character::end_fight() {
    m_cards_fight.erase(--m_cards_fight.end());
    m_current_state = StateCharacter::NORMAL_GAME;
    m_current_fight = nullptr;
}

void Character::end_fight_with_boss() {
    m_current_state = StateCharacter::NORMAL_GAME;
    m_current_fight = nullptr;
}

void to_json(nlohmann::json &json, const Character &character) {
    json["m_hand_limit"] = character.m_hand_limit;
    json["m_speed"] = character.m_speed;
    json["m_state_in_game"] = character.m_state_in_game;
    json["m_characteristics"] = character.m_characteristics;
    json["m_action_points"] = character.m_action_points;
    json["m_max_action_points"] = character.m_max_action_points;
    json["m_active_card_meeting"] = character.m_active_card_meeting;
    json["m_active_card_research"] = character.m_active_card_research;
    json["m_standard_character"] = character.m_standard_character;
    json["m_current_state"] = character.m_current_state;
    json["m_name"] = character.m_name;
    json["m_cards_research"] = character.m_cards_research;
    json["m_cards_fight"] = character.m_cards_fight;
    json["m_cards_meeting"] = character.m_cards_meeting;
    json["m_trophies"] = character.m_trophies;
    json["m_is_in_trade"] = character.m_is_in_trade;
    json["m_max_health"] = character.m_max_health;
    json["m_gold"] = character.m_gold;
    json["m_health"] = character.m_health;
    json["m_knowledge_token"] = character.m_knowledge_token;
    json["m_current_position"] = character.m_current_position;
    json["m_fight_tokens"] = character.m_fight_tokens;
    json["m_products"] = character.m_products;
}

void from_json(const nlohmann::json &json, Character &character) {
    character.m_hand_limit = json["m_hand_limit"];
    character.m_speed = json["m_speed"];
    character.m_state_in_game = json["m_state_in_game"];
    character.m_characteristics = json["m_characteristics"];
    character.m_action_points = json["m_action_points"];
    character.m_max_action_points = json["m_max_action_points"];
    character.m_standard_character = json["m_standard_character"];
    character.m_current_state = json["m_current_state"];
    character.m_name = json["m_name"];
    character.m_cards_research.clear();
    for (const auto &card : json["m_cards_research"]) {
        unsigned int card_int = card;
        character.m_cards_research.insert(card_int);
    }
    character.m_cards_fight.clear();
    for (const auto &card : json["m_cards_fight"]) {
        unsigned int card_int = card;
        character.m_cards_fight.insert(card_int);
    }
    character.m_cards_meeting.clear();
    for (const auto &card : json["m_cards_meeting"]) {
        unsigned int card_int = card;
        character.m_cards_meeting.insert(card_int);
    }
    character.m_trophies.clear();
    for (const auto &card : json["m_trophies"]) {
        std::pair<::runebound::AdventureType, unsigned int> card_int = card;
        character.m_trophies.insert(card_int);
    }
    character.m_is_in_trade = json["m_is_in_trade"];
    character.m_max_health = json["m_max_health"];
    character.m_gold = json["m_gold"];
    character.m_health = json["m_health"];
    character.m_knowledge_token = json["m_knowledge_token"];
    character.m_current_position = json["m_current_position"];
    character.m_fight_tokens.clear();
    for (const auto &fight_token : json["m_fight_tokens"]) {
        character.m_fight_tokens.push_back(fight_token);
    }
    character.m_products.clear();
    for (const auto &product : json["m_products"]) {
        unsigned int product_int = product;
        character.m_products.insert(product_int);
    }
}
}  // namespace character
}  // namespace runebound
