#include "doctest/doctest.h"
#include "game.hpp"

TEST_CASE("game") {
    ::runebound::game::Game game;
    using namespace runebound::fight;
    std::vector<FightToken> character_tokens = {
        FightToken(
            HandFightTokens::MAGICAL_DAMAGE, 0, HandFightTokens::DEXTERITY, 0
        ),
        FightToken(HandFightTokens::NOTHING, 1, HandFightTokens::DOUBLING, 0),
        FightToken(
            HandFightTokens::SHIELD, 0, HandFightTokens::PHYSICAL_DAMAGE, 0
        ),
        FightToken(
            HandFightTokens::PHYSICAL_DAMAGE, 0, HandFightTokens::NOTHING, 1
        ),
        FightToken(
            HandFightTokens::PHYSICAL_DAMAGE, 0,
            HandFightTokens::MAGICAL_DAMAGE, 1
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