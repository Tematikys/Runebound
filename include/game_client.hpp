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

    ::runebound::map::MapClient m_map;

    bool m_game_over = false;
    unsigned int m_turn = 0;
    unsigned int m_count_players = 0;
    unsigned int m_number_of_rounds = 0;
    unsigned int m_reward_gold_for_fight = 0;

    std::vector<::runebound::character::Character> m_characters;
    std::vector<::runebound::character::StandardCharacter>
        m_remaining_standard_characters;
    std::vector<character::StandardCharacter> m_free_characters;

    std::vector<dice::HandDice> m_last_dice_movement_result;
    std::vector<dice::HandDice> m_last_dice_relax_result;
    std::vector<dice::HandDice> m_last_dice_research_result;
    bool m_last_characteristic_check = false;
    std::vector<std::size_t> m_last_possible_outcomes;
    std::vector<Point> m_possible_moves;

    std::map<Point, std::set<unsigned int>> m_shops;
    std::vector<trade::Product> m_all_products;
    std::vector<cards::CardMeeting> m_all_cards_meeting;

    bool is_fight = false;
    runebound::fight::FightClient m_fight_client;

    ::runebound::character::StandardCharacter m_winner =
        ::runebound::character::StandardCharacter::NONE;

    GameClient() = default;

    explicit GameClient(const Game &game)
        : m_map(game.m_map),
          m_game_over(game.m_game_over),
          m_turn(game.m_turn),
          m_count_players(game.m_count_players),
          m_number_of_rounds(game.m_number_of_rounds),
          m_winner(game.get_winner()),
          m_characters(game.get_character_without_shared_ptr()),
          m_last_dice_movement_result(game.m_last_dice_movement_result),
          m_last_dice_relax_result(game.m_last_dice_relax_result),
          m_last_dice_research_result(game.m_last_dice_research_result),
          m_last_characteristic_check(game.m_last_characteristic_check),
          m_last_possible_outcomes(game.m_last_possible_outcomes),
          m_possible_moves(game.get_possible_moves()),
          m_shops(game.m_shops),
          m_all_products(game.m_all_products),
          m_all_cards_meeting(game.m_all_cards_meeting) {
        auto set_remaining =
            std::move(game.get_remaining_standard_characters());

        std::vector<::runebound::character::StandardCharacter> vec_remaining(
            set_remaining.begin(), set_remaining.end()
        );
        m_remaining_standard_characters = std::move(vec_remaining);

        auto set_free = game.m_free_characters;
        std::vector<::runebound::character::StandardCharacter> vec_free(
            set_free.begin(), set_free.end()
        );
        m_free_characters = std::move(vec_free);

        if (game.m_current_fight != nullptr) {
            m_reward_gold_for_fight =
                game.m_all_cards_fight[game.m_current_active_card_fight]
                    .get_gold_award();
        }

        if ((!game.m_characters.empty()) &&
            (game.m_characters[game.m_turn]->get_current_fight())) {
            is_fight = true;
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
