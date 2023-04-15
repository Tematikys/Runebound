#ifndef GAME_CLIENT_HPP_
#define GAME_CLIENT_HPP_

#include "game.hpp"
#include "map_client.hpp"
#include "runebound_fwd.hpp"

namespace runebound::game {
    void to_json(nlohmann::json &json, const GameClient &game);

    void from_json(const nlohmann::json &json, GameClient &game);

    struct GameClient {
    private:
        friend struct ::runebound::game::Game;
        ::runebound::map::MapClient m_map;
        std::vector<::runebound::character::Character>
                m_characters;
        std::vector<::runebound::character::StandardCharacter> m_remaining_standard_characters;
    public:
        GameClient() = default;

        explicit GameClient(const Game &game) : m_map(game.get_map())
                                               {
            auto set_remaining = game.get_remaining_standard_characters();
            std::vector<::runebound::character::StandardCharacter> vec_remaining (set_remaining.begin(), set_remaining.end());
            m_remaining_standard_characters=std::move(vec_remaining);

            for (const auto& p : game.get_characters()){
                m_characters.push_back(*p);
            }

        }

        friend void to_json(nlohmann::json &json, const GameClient &game);

        friend void from_json(const nlohmann::json &json, GameClient &game);
    };

}  // namespace runebound::game
#endif  // GAME_CLIENT_HPP_
