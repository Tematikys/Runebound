#include <iostream>
#include "client.hpp"

int main(int, char *[]) {
    ::runebound::client::Client client;

//    try {
//        client.init_network("localhost", "1234");
//        std::cout << "Connected\n";
//        std::cout << client.read_network();
//        client.write_network("get games\n");
//        ::std::cout << client.read_network();
//        client.write_network("add game Mygame\n");
//        client.write_network("get games\n");
//        ::std::cout << client.read_network();
//    } catch (std::exception &e) {
//        std::cerr << "Exception: " << e.what() << std::endl;
//    }

    client.init_graphics("Runebound-v0.0.7", 100, 100, 1920, 1080, 60);

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