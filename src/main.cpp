#include "game.hpp"
#include "runebound_fwd.hpp"

int main() {
    ::runebound::generator::generate_characters();
    ::runebound::generator::generate_cards_fight();
    ::runebound::game::Game game;
}