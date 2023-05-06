#include <graphics_client.hpp>

int main(int, char *[]) {
    ::runebound::graphics::Client client;
    client.init();

    while (client.is_running()) {
        client.handle_events();
        client.update();
        client.render();
        client.tick();
    }
    client.exit();

    return 0;
}