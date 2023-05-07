#include <vector>
#include "doctest/doctest.h"
#include "fight_commands.hpp"
#include "fight_two_player.hpp"
#include "game.hpp"
#include "runebound_fwd.hpp"

namespace runebound::tests {
void read_command(
    runebound::fight::FightTwoPlayer &fight,
    const std::vector<runebound::fight::TokenHandCount>
        &character_remaining_tokens,
    const std::vector<runebound::fight::TokenHandCount> &enemy_remaining_tokens
) {
    int k, d;
    using namespace runebound::fight;
    std::cin >> k;
    if (fight.get_turn() == runebound::fight::ParticipantTwoPlayers::CALLER) {
        if (character_remaining_tokens[k - 1].hand ==
            HandFightTokens::DOUBLING) {
            std::cout << "doubling\n";
            std::cin >> d;
            fight.make_doubling(
                ParticipantTwoPlayers::CALLER,
                character_remaining_tokens[k - 1],
                character_remaining_tokens[d - 1]
            );
        } else if (character_remaining_tokens[k - 1].hand == HandFightTokens::DEXTERITY) {
            int p;
            std::cout << "dexterity\n";

            std::cin >> d >> p;
            if (p == 0) {
                fight.make_dexterity(
                    ParticipantTwoPlayers::CALLER,
                    {character_remaining_tokens[k - 1]},
                    character_remaining_tokens[d - 1],
                    ParticipantTwoPlayers::CALLER
                );
            } else {
                fight.make_dexterity(
                    ParticipantTwoPlayers::CALLER,
                    character_remaining_tokens[k - 1],
                    enemy_remaining_tokens[d - 1],
                    ParticipantTwoPlayers::RECEIVER
                );
            }

        } else {
            std::cout << "damage\n";
            fight.make_damage(
                ParticipantTwoPlayers::CALLER,
                {character_remaining_tokens[k - 1]}
            );
        }
    } else {
        if (enemy_remaining_tokens[k - 1].hand == HandFightTokens::DOUBLING) {
            std::cout << "doubling\n";
            std::cin >> d;
            fight.make_doubling(
                ParticipantTwoPlayers::RECEIVER, enemy_remaining_tokens[k - 1],
                enemy_remaining_tokens[d - 1]
            );
        } else if (enemy_remaining_tokens[k - 1].hand == HandFightTokens::DEXTERITY) {
            int p;
            std::cout << "dexterity\n";
            std::cin >> d >> p;
            if (p == 0) {
                fight.make_dexterity(
                    ParticipantTwoPlayers::RECEIVER,
                    enemy_remaining_tokens[k - 1],
                    character_remaining_tokens[d - 1],
                    ParticipantTwoPlayers::CALLER
                );
            } else {
                fight.make_dexterity(
                    ParticipantTwoPlayers::RECEIVER,
                    enemy_remaining_tokens[k - 1],
                    enemy_remaining_tokens[d - 1],
                    ParticipantTwoPlayers::RECEIVER
                );
            }

        } else {
            std::cout << "damage\n";
            fight.make_damage(
                ParticipantTwoPlayers::RECEIVER, {enemy_remaining_tokens[k - 1]}
            );
        }
    }
}
}  // namespace runebound::tests

TEST_CASE("fight two players") {
    runebound::game::Game game;
    auto lissa =
        game.make_character(runebound::character::StandardCharacter::LISSA);
    auto mok =
        game.make_character(runebound::character::StandardCharacter::ELDER_MOK);
    runebound::fight::FightTwoPlayer fight(lissa, mok);
    CHECK(lissa->get_health() == 9);
    CHECK(mok->get_health() == 9);
    fight.start_round();
    runebound::fight::ParticipantTwoPlayers turn = fight.get_turn();
    auto caller_remaining_tokens = fight.get_caller_remaining_tokens();
    auto receiver_remaining_tokens = fight.get_receiver_remaining_tokens();
    while (true) {
        turn = fight.get_turn();
        if (fight.get_turn() ==
            runebound::fight::ParticipantTwoPlayers::CALLER) {
            std::cout << "LISSA turn\n";
        } else {
            std::cout << "MOK turn\n";
        }
        runebound::tests::print(caller_remaining_tokens);
        runebound::tests::print(receiver_remaining_tokens);
        std::cout << "LISSA HEALTH: " << lissa->get_health() << "; "
                  << "MOK HEALTH: " << mok->get_health() << '\n';
        if (fight.check_end_round()) {
            fight.start_round();
            caller_remaining_tokens = fight.get_caller_remaining_tokens();
            receiver_remaining_tokens = fight.get_receiver_remaining_tokens();
            continue;
        }
        runebound::tests::read_command(
            fight, caller_remaining_tokens, receiver_remaining_tokens
        );
        if (fight.check_end_fight()) {
            if (turn == runebound::fight::ParticipantTwoPlayers::CALLER) {
                std::cout << "Lissa win!\n";
            } else {
                std::cout << "Mok win!\n";
            }
            break;
        }
        caller_remaining_tokens = fight.get_caller_remaining_tokens();
        receiver_remaining_tokens = fight.get_receiver_remaining_tokens();
    }
}