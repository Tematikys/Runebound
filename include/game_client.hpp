#ifndef GAME_CLIENT_HPP_
#define GAME_CLIENT_HPP_

#include "fight_client.hpp"
#include "game.hpp"
#include "map_client.hpp"
#include "runebound_fwd.hpp"

namespace runebound::game {
void to_json(nlohmann::json &json, const GameClient &game);

void from_json(const nlohmann::json &json, GameClient &game);

struct GameClient {
public:
    friend struct ::runebound::game::Game;
    bool is_game_need_update = true;
    ::runebound::map::MapClient m_map;
    std::vector<::runebound::character::Character> m_characters;
    std::vector<::runebound::character::StandardCharacter>
        m_remaining_standard_characters;
    std::vector<dice::HandDice> m_last_dice_movement_result;
    bool is_fight = false;
    runebound::fight::FightClient m_fight_client;
    GameClient() = default;

    explicit GameClient(const Game &game)
        : m_map(game.get_map()),
          m_characters(game.get_character_without_shared_ptr()),
          m_last_dice_movement_result(game.get_last_dice_movement_result()) {
        auto set_remaining =
            std::move(game.get_remaining_standard_characters());
        std::vector<::runebound::character::StandardCharacter> vec_remaining(
            set_remaining.begin(), set_remaining.end()
        );
        m_remaining_standard_characters = std::move(vec_remaining);
        if ((game.m_characters.size() != 0) &&
            (game.m_characters[game.m_turn]->get_current_fight())) {
            is_fight = true;
            //            std::cout << "fight!!!\n";
            fight::FightClient fight_client(
                *game.m_characters[game.m_turn]->get_current_fight()
            );
            m_fight_client = fight_client;
        } else {
            is_fight = false;
        }
    }

    friend void to_json(nlohmann::json &json, const GameClient &game);

    friend void from_json(const nlohmann::json &json, GameClient &game);
};

}  // namespace runebound::game
#endif  // GAME_CLIENT_HPP_
