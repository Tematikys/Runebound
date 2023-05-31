#include "doctest/doctest.h"
#include "fight_two_player.hpp"
#include "game.hpp"

TEST_CASE("game") {
    ::runebound::generator::generate_characters();
    ::runebound::generator::generate_cards_fight();
    ::runebound::generator::generate_cards_meeting();
    ::runebound::generator::generate_cards_research();
    ::runebound::generator::generate_products();
    // ::runebound::generator::generate_map();
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
    auto fight = game.get_current_fight();
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
    auto lissa =
        game.make_character(runebound::character::StandardCharacter::LISSA);
    auto lord = game.make_character(
        runebound::character::StandardCharacter::LORD_HAWTHORNE
    );
    game.start_next_character_turn(lissa);
    lord->set_position(runebound::Point(13, 14));
    game.take_token(lord);
    CHECK(lord->get_state() == runebound::character::StateCharacter::FIGHT);
    CHECK(lissa->get_state() == runebound::character::StateCharacter::ENEMY);
    auto fight = game.get_current_fight();
    CHECK(lord->get_cards_fight().size() == 1);
    CHECK(lord->get_trophies().size() == 0);
    fight->get_enemy()->update_health(-fight->get_enemy()->get_health());
    CHECK(fight->check_end_fight() == true);
    game.end_fight(lord);
    CHECK(lord->get_cards_fight().size() == 0);
    CHECK(lord->get_trophies().size() == 1);
    CHECK(
        lord->get_state() == runebound::character::StateCharacter::NORMAL_GAME
    );
    CHECK(
        lissa->get_state() == runebound::character::StateCharacter::NORMAL_GAME
    );
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
    CHECK(outcomes == game.get_last_possible_outcomes());
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
    CHECK(game.get_last_possible_outcomes().empty());
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

TEST_CASE("trade") {
    runebound::game::Game game;
    auto mok =
        game.make_character(runebound::character::StandardCharacter::ELDER_MOK);
    mok->set_position(runebound::Point(10, 2));
    CHECK(mok->get_action_points() == 3);
    CHECK(mok->get_gold() == 2);
    auto products = game.get_town_products(mok->get_position());
    CHECK(products.size() == 3);
    game.start_trade(mok);
    CHECK(mok->get_action_points() == 2);
    mok->change_gold(100);
    CHECK(mok->get_gold() == 102);
    auto new_products = game.get_town_products(mok->get_position());
    CHECK(new_products.size() == 4);
    CHECK(mok->check_in_trade() == true);
    game.buy_product(mok, *(++new_products.begin()));
    CHECK(
        mok->get_gold() ==
        102 - game.get_product(*(++new_products.begin())).get_price()
    );
    CHECK(mok->check_in_trade() == false);
    CHECK(
        mok->get_standard_character() ==
        runebound::character::StandardCharacter::ELDER_MOK
    );
    CHECK(game.get_town_products(mok->get_position()).size() == 3);
    mok->set_position(runebound::Point(11, 13));
    game.start_trade(mok);
    CHECK(mok->get_action_points() == 1);
    CHECK(mok->check_in_trade() == true);
    auto mok_products = mok->get_products();
    CHECK(mok_products.size() == 1);
    auto pr = *mok_products.begin();
    if (game.get_product(pr).get_place_of_cell() ==
        runebound::map::SpecialTypeCell::NOTHING) {
        game.sell_product_in_town(mok, *mok_products.begin());
        CHECK(mok->get_gold() == 102);
    }
    game.discard_product(
        mok, *game.get_town_products(mok->get_position()).begin()
    );
    CHECK(mok->check_in_trade() == false);
}

TEST_CASE("time token") {
    runebound::game::Game game;
    auto lissa =
        game.make_character(runebound::character::StandardCharacter::LISSA);
    lissa->set_position(runebound::Point(0, 0));
    CHECK(
        game.get_map().get_cell_map(runebound::Point(0, 0)).get_side_token() ==
        runebound::Side::FRONT
    );
    game.take_token(lissa);
    CHECK(
        game.get_map().get_cell_map(runebound::Point(0, 0)).get_side_token() ==
        runebound::Side::BACK
    );
    CHECK(game.get_number_of_rounds() == 0);
    for (int i = 0; i < 5; ++i) {
        game.start_next_character_turn(lissa);
        CHECK(game.get_number_of_rounds() == i + 1);
        CHECK(
            game.get_map()
                .get_cell_map(runebound::Point(0, 0))
                .get_side_token() == runebound::Side::BACK
        );
    }
    game.start_next_character_turn(lissa);
    CHECK(game.get_number_of_rounds() == 6);
    CHECK(
        game.get_map().get_cell_map(runebound::Point(0, 0)).get_side_token() ==
        runebound::Side::FRONT
    );
}

TEST_CASE("fight two player") {
    runebound::game::Game game;
    auto lissa =
        game.make_character(runebound::character::StandardCharacter::LISSA);
    auto mok =
        game.make_character(runebound::character::StandardCharacter::ELDER_MOK);
    game.call_to_fight(lissa, mok);
    CHECK(lissa->get_action_points() == 2);
    CHECK(mok->get_action_points() == 3);
    CHECK(
        lissa->get_state() == runebound::character::StateCharacter::NORMAL_GAME
    );
    CHECK(
        mok->get_state() == runebound::character::StateCharacter::NORMAL_GAME
    );
    game.accept_to_fight(mok);
    auto fight = game.get_current_fight_two_player();
    CHECK(fight == mok->get_current_fight_two_player());
    CHECK(lissa->get_state() == runebound::character::StateCharacter::CALLER);
    CHECK(mok->get_state() == runebound::character::StateCharacter::RECEIVER);
    mok->update_health(-9);
    CHECK(fight->check_end_fight() == true);
    CHECK(
        fight->get_winner() == runebound::fight::ParticipantTwoPlayers::CALLER
    );
    game.end_fight_two_player(lissa);
    CHECK(
        lissa->get_state() == runebound::character::StateCharacter::NORMAL_GAME
    );
    CHECK(
        mok->get_state() == runebound::character::StateCharacter::NORMAL_GAME
    );
    CHECK(mok->get_current_fight_two_player() == nullptr);
    CHECK(game.get_current_fight_two_player() == nullptr);
    CHECK(lissa->get_current_fight_two_player() == nullptr);
    CHECK(mok->get_current_caller_to_fight() == nullptr);
}

TEST_CASE("fight boss") {
    runebound::fight::Enemy boss =
        runebound::fight::Enemy(runebound::AdventureType::BOSS);
    CHECK(boss.get_fight_token().size() == 8);
    CHECK(boss.get_health() == 15);
    boss.update_health(-4);
    CHECK(boss.get_health() == 11);
    boss.make_hit();
    CHECK(boss.get_health() == 14);
    boss.make_hit();
    CHECK(boss.get_health() == 15);
    runebound::game::Game game;
    auto lissa =
        game.make_character(runebound::character::StandardCharacter::LISSA);
    lissa->change_knowledge_token(6);
    runebound::fight::Fight fight(
        lissa, runebound::fight::Enemy(runebound::AdventureType::BOSS)
    );
    CHECK(fight.get_health_enemy() == 15);
    fight.start_round();
    CHECK(fight.get_health_enemy() == 9);
}

TEST_CASE("boss in game") {
    runebound::game::Game game;
    auto corbin =
        game.make_character(runebound::character::StandardCharacter::CORBIN);
    bool boss_in_game = false;
    auto map = game.get_map();
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            if (map.get_cell_map(runebound::Point(i, j)).get_token() ==
                runebound::AdventureType::BOSS) {
                boss_in_game = true;
            }
        }
    }
    CHECK(!boss_in_game);
    for (int i = 0; i < 11; ++i) {
        game.start_next_character_turn(corbin);
    }
    map = game.get_map();
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            if (map.get_cell_map(runebound::Point(i, j)).get_token() ==
                runebound::AdventureType::BOSS) {
                boss_in_game = true;
            }
        }
    }
    CHECK(!boss_in_game);
    game.start_next_character_turn(corbin);
    CHECK(game.get_number_of_rounds() == 12);
    CHECK(
        game.get_map().get_cell_map(runebound::Point(11, 6)).get_token() ==
        runebound::AdventureType::BOSS
    );
    for (int i = 0; i < 12; ++i) {
        game.start_next_character_turn(corbin);
        CHECK(
            game.get_map().get_cell_map(runebound::Point(11, 6)).get_token() ==
            runebound::AdventureType::BOSS
        );
        CHECK(game.check_end_game() == false);
    }
    for (int i = 0; i < 5; ++i) {
        game.start_next_character_turn(corbin);
        CHECK(
            game.get_map()
                .get_cell_map(runebound::Point(11 - i - 1, 6))
                .get_token() == runebound::AdventureType::BOSS
        );
        CHECK(game.check_end_game() == false);
    }
    game.start_next_character_turn(corbin);
    CHECK(game.check_end_game() == true);
}

TEST_CASE("fight with boss in game") {
    runebound::game::Game game;
    auto corbin =
        game.make_character(runebound::character::StandardCharacter::CORBIN);
    for (int i = 0; i < 12; ++i) {
        game.start_next_character_turn(corbin);
    }
    CHECK(
        game.get_map().get_cell_map(runebound::Point(11, 6)).get_token() ==
        runebound::AdventureType::BOSS
    );
    corbin->set_position(runebound::Point(11, 6));
    auto lord = game.make_character(
        runebound::character::StandardCharacter::LORD_HAWTHORNE
    );
    game.take_token(corbin);
    auto fight = game.get_current_fight();
    CHECK(corbin->get_state() == runebound::character::StateCharacter::FIGHT);
    CHECK(lord->get_state() == runebound::character::StateCharacter::ENEMY);
    CHECK(fight != nullptr);
    CHECK(fight->get_health_enemy() == 15);
    corbin->update_health(-corbin->get_health());
    CHECK(fight->check_end_fight() == true);
    CHECK(fight->get_winner() == runebound::fight::Participant::ENEMY);
    game.end_fight_with_boss(corbin);
    CHECK(game.check_end_game() == false);
    CHECK(game.get_winner() == runebound::character::StandardCharacter::NONE);
    CHECK(corbin->get_health() == 0);
    CHECK(
        corbin->get_state() == runebound::character::StateCharacter::NORMAL_GAME
    );
    CHECK(
        lord->get_state() == runebound::character::StateCharacter::NORMAL_GAME
    );
    game.start_next_character_turn(corbin);
    lord->set_position(runebound::Point(11, 6));
    game.take_token(lord);
    auto lord_fight = game.get_current_fight();
    CHECK(corbin->get_state() == runebound::character::StateCharacter::ENEMY);
    CHECK(lord->get_state() == runebound::character::StateCharacter::FIGHT);
    CHECK(lord_fight->get_health_enemy() == 15);
    lord_fight->get_enemy()->update_health(-15);
    CHECK(lord_fight->get_winner() == runebound::fight::Participant::CHARACTER);
    game.end_fight_with_boss(lord);
    CHECK(game.check_end_game() == true);
    CHECK(
        game.get_winner() ==
        runebound::character::StandardCharacter::LORD_HAWTHORNE
    );
    CHECK(
        corbin->get_state() == runebound::character::StateCharacter::NORMAL_GAME
    );
    CHECK(
        lord->get_state() == runebound::character::StateCharacter::NORMAL_GAME
    );
}

TEST_CASE("neighbours cells") {
    runebound::game::Game game;
    auto lissa =
        game.make_character(runebound::character::StandardCharacter::LISSA);
    CHECK(lissa->get_position() == runebound::Point(8, 13));
    CHECK(game.get_possible_moves().size() == 6);
    game.throw_movement_dice(lissa);
    CHECK(game.get_possible_moves().size() > 6);
}

TEST_CASE("to_json from_json game") {
    runebound::game::Game game;
    auto lissa =
        game.make_character(runebound::character::StandardCharacter::LISSA);
    auto mok =
        game.make_character(runebound::character::StandardCharacter::ELDER_MOK);
    mok->set_position(runebound::Point(11, 1));
    nlohmann::json json, json_after;
    runebound::game::to_json(json, game);
    runebound::game::Game game_after_json;
    runebound::game::from_json(json, game_after_json);
    runebound::game::to_json(json_after, game_after_json);
    CHECK(json == json_after);
}