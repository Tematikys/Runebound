#include "game_client.hpp"
#include <nlohmann/json.hpp>
#include "character.hpp"
#include "character_client.hpp"
#include "map_client.hpp"

namespace runebound::game {

void to_json(nlohmann::json &json, const GameClient &game) {
    json["m_map"] = game.m_map;

    json["m_game_over"] = game.m_game_over;
    json["m_turn"] = game.m_turn;
    json["m_count_players"] = game.m_count_players;
    json["m_number_of_rounds"] = game.m_number_of_rounds;

    json["m_characters"] = game.m_characters;
    json["m_remaining_standard_characters"] =
        game.m_remaining_standard_characters;

    json["m_last_dice_movement_result"] = game.m_last_dice_movement_result;
    json["m_last_dice_relax_result"] = game.m_last_dice_relax_result;
    json["m_last_dice_research_result"] = game.m_last_dice_research_result;
    json["m_last_characteristic_check"] = game.m_last_characteristic_check;
    json["m_last_possible_outcomes"] = game.m_last_possible_outcomes;

    json["is_fight"] = game.is_fight;
    if (game.is_fight) {
        json["m_fight_client"] = game.m_fight_client;
    }
}

void from_json(const nlohmann::json &json, GameClient &game) {
    game.m_map = json["m_map"];

    game.m_game_over = json["m_game_over"];
    game.m_turn = json["m_turn"];
    game.m_count_players = json["m_count_players"];
    game.m_number_of_rounds = json["m_number_of_rounds"];

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
    game.m_last_dice_relax_result = std::vector<::runebound::dice::HandDice>(
        json["m_last_dice_relax_result"].begin(),
        json["m_last_dice_relax_result"].end()
    );
    game.m_last_dice_research_result = std::vector<::runebound::dice::HandDice>(
        json["m_last_dice_research_result"].begin(),
        json["m_last_dice_research_result"].end()
    );

    game.m_last_characteristic_check = std::vector<unsigned int>(
        json["m_last_characteristic_check"].begin(),
        json["m_last_characteristic_check"].end()
    );

    game.m_last_possible_outcomes = std::vector<std::size_t>(
        json["m_last_possible_outcomes"].begin(),
        json["m_last_possible_outcomes"].end()
    );

    game.is_fight = json["is_fight"];
    if (game.is_fight) {
        game.m_fight_client = json["m_fight_client"];
    }
}
}  // namespace runebound::game