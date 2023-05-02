#include "game_client.hpp"
#include <nlohmann/json.hpp>
#include "character.hpp"
#include "character_client.hpp"
#include "map_client.hpp"
#include "tokens.hpp"

namespace runebound::game {

void to_json(nlohmann::json &json, const GameClient &game) {
    json["m_characters"] = game.m_characters;
    json["m_map"] = game.m_map;
    json["m_remaining_standard_characters"] =
        game.m_remaining_standard_characters;
    json["m_last_dice_movement_result"] = game.m_last_dice_movement_result;
}

void from_json(const nlohmann::json &json, GameClient &game) {
    game.m_map = json["m_map"];
    game.m_characters = json["m_characters"];
    game.m_remaining_standard_characters =
        std::vector<::runebound::character::StandardCharacter>(
            json["m_remaining_standard_characters"].begin(),
            json["m_remaining_standard_characters"].end()
        );
    game.m_last_dice_movement_result = std::vector<::runebound::dice::HandDice>(
        json["m_last_dice_movement_result"].begin(),
        json["m_last_dice_movement_result"].end()
    );
}

}  // namespace runebound::game