#include "client.hpp"

int main(int, char *[]) {
    ::runebound::client::Client client;

    client.init_graphics("Runebound-v0.0.8", 100, 100, 900, 800, 60);

    const ::runebound::map::Map map;
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