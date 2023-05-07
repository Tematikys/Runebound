#include <utility>
#include "doctest/doctest.h"
#include "fight.hpp"
#include "fight_commands.hpp"
#include "game.hpp"

namespace runebound::tests {
void read_command(
    runebound::fight::Fight &fight,
    const std::vector<runebound::fight::TokenHandCount>
        &character_remaining_tokens,
    const std::vector<runebound::fight::TokenHandCount> &enemy_remaining_tokens
) {
    int k, d;
    using namespace runebound::fight;
    std::cin >> k;
    if (fight.get_turn() == runebound::fight::Participant::CHARACTER) {
        if (character_remaining_tokens[k - 1].hand ==
            HandFightTokens::DOUBLING) {
            std::cout << "doubling\n";
            std::cin >> d;
            fight.make_doubling(
                Participant::CHARACTER, character_remaining_tokens[k - 1],
                character_remaining_tokens[d - 1]
            );
        } else if (character_remaining_tokens[k - 1].hand == HandFightTokens::DEXTERITY) {
            int p;
            std::cout << "dexterity\n";

            std::cin >> d >> p;
            if (p == 0) {
                fight.make_dexterity(
                    Participant::CHARACTER, {character_remaining_tokens[k - 1]},
                    character_remaining_tokens[d - 1], Participant::CHARACTER
                );
            } else {
                fight.make_dexterity(
                    Participant::CHARACTER, character_remaining_tokens[k - 1],
                    enemy_remaining_tokens[d - 1], Participant::ENEMY
                );
            }

        } else {
            std::cout << "damage\n";
            fight.make_damage(
                Participant::CHARACTER, {character_remaining_tokens[k - 1]}
            );
        }
    } else {
        if (enemy_remaining_tokens[k - 1].hand == HandFightTokens::DOUBLING) {
            std::cout << "doubling\n";
            std::cin >> d;
            fight.make_doubling(
                Participant::ENEMY, enemy_remaining_tokens[k - 1],
                enemy_remaining_tokens[d - 1]
            );
        } else if (enemy_remaining_tokens[k - 1].hand == HandFightTokens::DEXTERITY) {
            int p;
            std::cout << "dexterity\n";
            std::cin >> d >> p;
            if (p == 0) {
                fight.make_dexterity(
                    Participant::ENEMY, enemy_remaining_tokens[k - 1],
                    character_remaining_tokens[d - 1], Participant::CHARACTER
                );
            } else {
                fight.make_dexterity(
                    Participant::ENEMY, enemy_remaining_tokens[k - 1],
                    enemy_remaining_tokens[d - 1], Participant::ENEMY
                );
            }

        } else {
            std::cout << "damage\n";
            fight.make_damage(
                Participant::ENEMY, {enemy_remaining_tokens[k - 1]}
            );
        }
    }
}

}  // namespace runebound::tests

TEST_CASE("Fight") {
    using namespace runebound::fight;
    ::runebound::game::Game game;
    std::vector<FightToken> character_tokens = {
        FightToken(
            HandFightTokens::MAGICAL_DAMAGE, 0, 1, HandFightTokens::DEXTERITY,
            0, 1
        ),
        FightToken(
            HandFightTokens::NOTHING, 1, 1, HandFightTokens::DOUBLING, 0, 1
        ),
        FightToken(
            HandFightTokens::SHIELD, 0, 1, HandFightTokens::PHYSICAL_DAMAGE, 0,
            1
        ),
        FightToken(
            HandFightTokens::PHYSICAL_DAMAGE, 0, 1, HandFightTokens::NOTHING, 1,
            1
        ),
        FightToken(
            HandFightTokens::PHYSICAL_DAMAGE, 0, 1,
            HandFightTokens::MAGICAL_DAMAGE, 1, 1
        ),  // THERE HIT
    };

    std::shared_ptr<runebound::character::Character> character =
        game.make_character(
            100, 7, ::runebound::Point(0, 0), 5, 5, "Katya", character_tokens
        );
    auto enemy = Enemy(5, "ABC");
    Fight fight = Fight(character, enemy);
    fight.start_round();
    Participant turn = fight.get_turn();
    auto character_remaining_tokens = fight.get_character_remaining_tokens();
    auto enemy_remaining_tokens = fight.get_enemy_remaining_tokens();
    while (true) {
        Participant turn = fight.get_turn();
        if (fight.get_turn() == runebound::fight::Participant::CHARACTER) {
            std::cout << "CHARACTER turn\n";
        } else {
            std::cout << "ENEMY turn\n";
        }
        runebound::tests::print(character_remaining_tokens);
        runebound::tests::print(enemy_remaining_tokens);
        std::cout << "CHARACTER HEALTH: " << character->get_health() << "; "
                  << "ENEMY HEALTH: " << fight.get_health_enemy() << '\n';
        if (fight.check_end_round()) {
            fight.start_round();
            character_remaining_tokens = fight.get_character_remaining_tokens();
            enemy_remaining_tokens = fight.get_enemy_remaining_tokens();
            continue;
        }
        runebound::tests::read_command(
            fight, character_remaining_tokens, enemy_remaining_tokens
        );
        if (fight.check_end_fight()) {
            if (turn == runebound::fight::Participant::ENEMY) {
                std::cout << "ENEMY win!\n";
            } else {
                std::cout << "CHARACTER win!\n";
            }
            break;
        }
        character_remaining_tokens = fight.get_character_remaining_tokens();
        enemy_remaining_tokens = fight.get_enemy_remaining_tokens();
    }
}