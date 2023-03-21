#include <SDL2/SDL.h>
#include <iostream>
#include "client.hpp"

int main(int /*argc*/, char * /*args*/[]) {
    auto client = new ::runebound::client::Client;

    client->init_graphics("Runebound-v0.0.6.1", 100, 100, 640, 480, 165);

    const ::runebound::map::Map map;
    client->init_board(map);

    while (client->running()) {
        client->handle_events();
        client->update();
        client->render();
        client->tick();
    }

    client->exit();
    return 0;
}