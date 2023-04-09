#include "doctest/doctest.h"
#include "game.hpp"
#include "fight.hpp"
#include <utility>

void print(const std::vector<runebound::fight::TokenHandCount> tokens) {
    for (auto token : tokens) {
        switch ( token.hand) {
            case (runebound::fight::HandFightTokens::MAGICAL_DAMAGE): {
                std::cout << "MAGICAL\n";
                break;
            }
            case (runebound::fight::HandFightTokens::PHYSICAL_DAMAGE): {
                std::cout << "PHYSICAL\n";
                break;
            }
            case (runebound::fight::HandFightTokens::ENEMY_DAMAGE): {
                std::cout << "ENEMY\n";
                break;
            }
            case (runebound::fight::HandFightTokens::DOUBLING): {
                std::cout << "DOUBLING\n";
                break;
            }
            case (runebound::fight::HandFightTokens::DEXTERITY): {
                std::cout << "DEXTERITY\n";
                break;
            }
            case (runebound::fight::HandFightTokens::SHIELD): {
                std::cout << "SHIELD\n";
                break;
            }
            case (runebound::fight::HandFightTokens::NOTHING): {
                std::cout << "NOTHING\n";
                break;
            }
        }
    }
    std::cout << '\n';
}

void read_command(runebound::fight::Fight &fight,
                  const std::vector<runebound::fight::TokenHandCount> &character_remaining_tokens,
                  const std::vector<runebound::fight::TokenHandCount> &enemy_remaining_tokens) {
    int k;
    using namespace runebound::fight;
    std::cin >> k;
    if (fight.get_turn() == runebound::fight::Participant::CHARACTER) {
        fight.make_progress(Participant::CHARACTER, {character_remaining_tokens[k - 1]}, std::nullopt, std::nullopt, std::nullopt);
    }
    else {
        fight.make_progress(Participant::ENEMY, {enemy_remaining_tokens[k - 1]}, std::nullopt, std::nullopt, std::nullopt);
    }
}
TEST_CASE("Fight") {
    using namespace runebound::fight;
    ::runebound::game::Game game;
    std::vector <FightToken> character_tokens = {
        FightToken(HandFightTokens::MAGICAL_DAMAGE, 0, HandFightTokens::DEXTERITY, 0),
        FightToken(HandFightTokens::NOTHING, 1, HandFightTokens::DOUBLING, 0),
        FightToken(HandFightTokens::SHIELD, 0, HandFightTokens::PHYSICAL_DAMAGE, 0),
        FightToken(HandFightTokens::PHYSICAL_DAMAGE, 0, HandFightTokens::NOTHING, 1),
        FightToken(HandFightTokens::PHYSICAL_DAMAGE, 0, HandFightTokens::MAGICAL_DAMAGE, 1), // THERE HIT
    };



    std::shared_ptr<runebound::character::Character> character = game.make_character(100, 10, ::runebound::Point(0, 0), 5, 5, "Katya", character_tokens);
    auto enemy = Enemy(7, "ABC");
    Fight fight = Fight(character, enemy);
    fight.start_round();
    Participant turn = fight.get_turn();
    auto character_remaining_tokens = fight.get_character_remaining_tokens();
    auto enemy_remaining_tokens = fight.get_enemy_remaining_tokens();
    while  (true) {
        if (fight.get_turn() == runebound::fight::Participant::CHARACTER) {
            std::cout << "CHARACTER\n";
        }
        else {
            std::cout << "ENEMY\n";
        }
        print(character_remaining_tokens);
        print(enemy_remaining_tokens);
        if (character_remaining_tokens.size() == 0 && enemy_remaining_tokens.size() == 0) {
            fight.start_round();
        }
        read_command(fight, character_remaining_tokens, enemy_remaining_tokens);
        if (fight.check_end_fight()) {
            if (fight.get_turn() == runebound::fight::Participant::CHARACTER) {
                std::cout << "ENEMY win!";
            }
            else {
                std::cout << "CHARACTER win!";
            }
            break;
        }
        character_remaining_tokens = fight.get_character_remaining_tokens();
        enemy_remaining_tokens = fight.get_enemy_remaining_tokens();
    }
}