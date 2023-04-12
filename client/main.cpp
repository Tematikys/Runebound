#include "client.hpp"

int main(int, char *[]) {
    ::runebound::client::Client client;
    client.init();

    while (client.running()) {
        client.handle_events();
        client.update();
        client.render();
        client.tick();
    }
    client.exit();

    return 0;
}