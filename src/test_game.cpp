#include "doctest/doctest.h"
#include "game.hpp"

TEST_CASE("game") {
    ::runebound::game::Game game;
    using namespace runebound::fight;
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
        ),
    };

    std::shared_ptr<runebound::character::Character> first =
        game.make_character(
            100, 7, runebound::Point(0, 0), 4, 2, "Bogdan", character_tokens
        );
    std::shared_ptr<runebound::character::Character> second =
        game.make_character(
            100, 7, runebound::Point(10, 7), 4, 2, "Artem", character_tokens
        );
    CHECK(game.get_turn() == 0);
    game.relax(first);
    CHECK(first->get_action_points() == 2);
    CHECK(game.get_turn() == 0);
    std::vector<runebound::dice::HandDice> dice_res{
        runebound::dice::HandDice::JOKER};
    game.make_move(first, ::runebound::Point(0, 1), dice_res);
    CHECK(game.get_position_character(first) == runebound::Point(0, 1));
    CHECK(first->get_action_points() == 0);
    game.start_next_character_turn();
    CHECK(game.get_turn() == 1);
    CHECK(second->get_action_points() == 3);
    auto third = game.make_character(
        100, 7, runebound::Point(0, 4), 4, 2, "Katya", character_tokens
    );
    game.start_next_character_turn();
    CHECK(game.get_turn() == 2);
    game.reverse_token(third);
    CHECK(third->get_state() == runebound::character::StateCharacter::FIGHT);
    auto fight = third->get_current_fight();
    CHECK(fight != nullptr);
}

TEST_CASE("generating characters") {
    ::runebound::game::Game game;
    auto remaining_characters = game.get_remaining_standard_characters();
    CHECK(remaining_characters.size() == 6);
    auto lissa =
        game.make_character(runebound::character::StandardCharacter::LISSA);
    remaining_characters = game.get_remaining_standard_characters();
    CHECK(remaining_characters.size() == 5);
    CHECK(lissa->get_name() == "Lissa");
    auto corbin =
        game.make_character(runebound::character::StandardCharacter::CORBIN);
    CHECK(
        corbin->get_state() == runebound::character::StateCharacter::NORMAL_GAME
    );
    remaining_characters = game.get_remaining_standard_characters();
    CHECK(remaining_characters.size() == 4);
    CHECK(corbin->get_name() == "Corbin");
    std::vector<runebound::dice::HandDice> dice_res{
        runebound::dice::HandDice::JOKER};
    CHECK(lissa->get_action_points() == 3);
    game.make_move(lissa, runebound::Point(9, 13), dice_res);
    CHECK(lissa->get_action_points() == 1);
    game.relax(lissa);
    CHECK(lissa->get_action_points() == 0);
    game.start_next_character_turn();
    CHECK(corbin->get_action_points() == 3);
    game.make_move(corbin, runebound::Point(12, 6), dice_res);
    CHECK(corbin->get_action_points() == 1);
}