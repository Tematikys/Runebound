#include "game_client.hpp"
#include <nlohmann/json.hpp>
#include "character_client.hpp"
#include "map_client.hpp"
#include "tokens.hpp"

namespace runebound::game {

void to_json(nlohmann::json &json, const GameClient &game) {
    // json["m_characters"] = game.m_characters;
    json["m_map"] = game.m_map;
}

void from_json(const nlohmann::json &json, GameClient &game) {
    game.m_map = json["m_map"];
    // game.m_characters = json["m_characters"];
}

}  // namespace runebound::game