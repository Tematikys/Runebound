#include "game.hpp"
#include "map_client.hpp"
#include "game_client.hpp"

int main() {
    runebound::game::Game game;
    runebound::map::Map map;
    runebound::map::MapClient map_client;
    runebound::game::GameClient game_client;
    // std::cout << map.get_cell_map(::runebound::Point(2,
    // 2)).get_territory_name();
}