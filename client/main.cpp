#include "client.hpp"

int main(int, char *[]) {
    ::runebound::client::Client client;

    client.init_graphics("Runebound-v0.1.4", 100, 100, 1000, 1000, 60);

    const ::runebound::map::MapClient map;
    client.init_board(map);

    while (client.running()) {
        client.handle_events();
        client.update();
        client.render();
        client.tick();
    }
    client.exit();

    return 0;
}