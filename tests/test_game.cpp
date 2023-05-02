#include "doctest/doctest.h"
#include "game.hpp"
#include "nlohmann/json"

TEST_CASE("game") {
    ::runebound::generator::generate_characters();
    ::runebound::generator::generate_cards_fight();
    ::runebound::generator::generate_cards_meeting();
    ::runebound::generator::generate_cards_research();
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
    CHECK(first->get_action_points() == 1);
    game.start_next_character_turn(first);
    CHECK(game.get_turn() == 1);
    CHECK(second->get_action_points() == 3);
    auto third = game.make_character(
        100, 7, runebound::Point(0, 4), 4, 2, "Katya", character_tokens
    );
    game.start_next_character_turn(second);
    CHECK(game.get_turn() == 2);
    game.take_token(third);
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
    CHECK(lissa->get_action_points() == 2);
    game.relax(lissa);
    CHECK(lissa->get_action_points() == 1);
    game.start_next_character_turn(lissa);
    CHECK(corbin->get_action_points() == 3);
    auto res = game.throw_movement_dice(corbin);
    CHECK(res == game.get_last_dice_movement_result());
    CHECK(static_cast<unsigned int>(res.size()) == corbin->get_speed());
    game.make_move(corbin, runebound::Point(12, 6), res);
    CHECK(corbin->get_action_points() == 2);
    CHECK(
        corbin->get_standard_character() ==
        runebound::character::StandardCharacter::CORBIN
    );
    CHECK(
        lissa->get_standard_character() ==
        runebound::character::StandardCharacter::LISSA
    );
    CHECK(0 == game.get_last_dice_movement_result().size());
}

TEST_CASE("card_fight") {
    ::runebound::game::Game game;
    auto lord = game.make_character(
        runebound::character::StandardCharacter::LORD_HAWTHORNE
    );
    game.relax(lord);
    lord->set_position(runebound::Point(13, 14));
    CHECK(lord->get_action_points() == 2);
    game.start_next_character_turn(lord);
    game.take_token(lord);
    CHECK(lord->get_state() == runebound::character::StateCharacter::FIGHT);
    auto fight = lord->get_current_fight();
    CHECK(lord->get_cards_fight().size() == 1);
    CHECK(lord->get_trophies().size() == 0);
    fight->get_enemy()->update_health(-fight->get_enemy()->get_health());
    fight->check_end_fight();
    game.end_fight(lord);
    CHECK(lord->get_cards_fight().size() == 0);
    CHECK(lord->get_trophies().size() == 1);
}

TEST_CASE("cards") {
    ::runebound::game::Game game;
    auto lord = game.make_character(
        runebound::character::StandardCharacter::LORD_HAWTHORNE
    );
    lord->set_position(runebound::Point(13, 0));
    game.take_token(lord);
    CHECK(lord->get_action_points() == 1);
    auto card = *(lord->get_cards(runebound::AdventureType::RESEARCH).begin());
    auto copy_card = game.get_card_research(card);
    auto territory = copy_card.get_required_territory();
    auto cells = game.get_territory_cells(territory);
    lord->set_position(cells[0]);
    game.start_card_execution(lord, card, runebound::AdventureType::RESEARCH);
    CHECK(card == lord->get_active_card_research());
    game.throw_research_dice(lord);
    CHECK(game.get_last_dice_movement_result().size() == 0);
    CHECK(game.get_last_dice_research_result().size() == 3);
    auto outcomes = game.get_possible_outcomes(lord);
    CHECK(lord->get_cards(runebound::AdventureType::RESEARCH).size() == 1);
    if (outcomes.empty()) {
        game.complete_card_research(lord, -1);
    } else {
        game.complete_card_research(lord, outcomes.back());
    }
    int trophies = 0;
    if (outcomes.size() > 0) {
        CHECK(lord->get_trophies().size() == 1);
        trophies = 1;
    } else {
        CHECK(lord->get_trophies().size() == 0);
    }
    CHECK(lord->get_cards(runebound::AdventureType::RESEARCH).size() == 0);
    CHECK(lord->get_action_points() == 1);
    CHECK(game.get_last_dice_movement_result().size() == 0);
    CHECK(game.get_last_dice_research_result().size() == 0);
    game.start_next_character_turn(lord);
    CHECK(lord->get_action_points() == 3);
    lord->set_position(runebound::Point(0, 0));
    game.take_token(lord);
    CHECK(lord->get_action_points() == 1);
    auto card_meeting =
        *(lord->get_cards(runebound::AdventureType::MEETING).begin());
    game.start_card_execution(
        lord, card_meeting, runebound::AdventureType::MEETING
    );
    if (game.check_characteristic(
            lord, card, runebound::cards::OptionMeeting::FIRST
        )) {
        CHECK(lord->get_trophies().size() == trophies + 1);
    } else {
        CHECK(lord->get_trophies().size() == trophies);
    }
}
