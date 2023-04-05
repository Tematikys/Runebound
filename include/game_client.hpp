#ifndef GAME_CLIENT_HPP_
#define GAME_CLIENT_HPP_

#include "map_client.hpp"
#include "runebound_fwd.hpp"

namespace runebound::game {
void to_json(nlohmann::json &json, const GameClient &game);
void from_json(const nlohmann::json &json, GameClient &game);

struct GameClient {
private:
    ::runebound::map::MapClient m_map;
    // std::vector <::runebound::character::CharacterClient> m_characters;
public:
    friend void to_json(nlohmann::json &json, const GameClient &game);
    friend void from_json(const nlohmann::json &json, GameClient &game);
};

}  // namespace runebound::game
#endif  // GAME_CLIENT_HPP_
