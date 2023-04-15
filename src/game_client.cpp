#include "game_client.hpp"
#include <nlohmann/json.hpp>
#include "character_client.hpp"
#include "map_client.hpp"
#include "tokens.hpp"
#include "character.hpp"

namespace runebound::game {

    void to_json(nlohmann::json &json, const GameClient &game) {
        json["m_characters"] = game.m_characters;
        json["m_map"] = game.m_map;
        json["m_remaining_standard_characters"] = game.m_remaining_standard_characters;
    }

    void from_json(const nlohmann::json &json, GameClient &game) {
        game.m_map = json["m_map"];
        game.m_characters = json["m_characters"];
        game.m_remaining_standard_characters = std::vector<::runebound::character::StandardCharacter>(
                json["m_remaining_standard_characters"].begin(), json["m_remaining_standard_characters"].end());
    }

}  // namespace runebound::game