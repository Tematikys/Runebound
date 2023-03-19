#include <algorithm>
#include <fstream>
#include <iostream>
#include <thread>
#include <utility>


#include "game.hpp"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

#ifdef _MSC_VER
#include <crtdbg.h>
#endif

struct user;

runebound::game::Game game();
std::set<user*> users;

struct user {
    user(tcp::iostream io, std::string username) : io(std::move(io)), username(username) {
//        character = character();
        users.insert(this);
    };

    void disconnected() {users.erase(this);}
    runebound::character::Character *character;
    tcp::iostream io;
    std::string username;
};







int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr
                << "Usage: "
                << std::string(argv[0])
                << " <port> \n";
        return 1;
    }



    try {
        boost::asio::io_context io_context;
        tcp::acceptor acceptor(
                io_context,
                tcp::endpoint(tcp::v4(),static_cast<short>(std::stoi(argv[1]))
                )
        );
        std::cout << "Listening at " << acceptor.local_endpoint() << "\n";

        while (true) {
            tcp::socket s = acceptor.accept();
            std::thread([s = std::move(s)]() mutable {
                tcp::endpoint remote = s.remote_endpoint();
                tcp::endpoint local = s.local_endpoint();
                std::cout << "Connected " << remote << " --> " << local << "\n";
                tcp::iostream client_io(std::move(s));
                client_io << "What is your name?\n";
                std::string username;
                client_io >> username;
                client_io << "Hi " << username << "\n";
                user user(std::move(client_io), username);
                while (client_io) {
                    try {
                        std::string command;
                        if (!(client_io >> command)) {
                            break;
                        }
                        if (command=="move"){
                            int x,y;
                            client_io>>x>>y;
//                            game.make_move(user, x, y, )

                        }
                        client_io << "Unknown command: '" << command << "'\n";
                    } catch (std::exception &e) {
                        std::cout << e.what() << "\n";
                    }
                }
                user.disconnected();
                std::cout << "Disconnected " << remote << " --> " << local
                          << "\n";
            }).detach();
        }

    } catch (...) {
    }
    return 0;
}