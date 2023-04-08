#include "doctest/doctest.h"
#include "game.hpp"

TEST_CASE("Fight") {
    ::runebound::game::Game game;
    auto character = game.make_character(100, 10, ::runebound::Point(0, 0), 5, 5, "Katya");

}