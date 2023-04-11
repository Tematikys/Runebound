#include <iostream>
#include <boost/asio.hpp>

#include <chrono>
#include <thread>
#include <nlohmann/json.hpp>
#include "client.hpp"

using boost::asio::ip::tcp;
using json = nlohmann::json;


int main() {
    std::string user_name;
    std::cout << "Write your name:\n";
    std::cin >> user_name;

    boost::asio::io_context io_context;
    auto work_guard = boost::asio::make_work_guard(io_context);
    Client client(io_context, "127.0.0.1", 4444, user_name);
    try {
        //test commands begin
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        io_context.poll();

        std::cout << "List of games:\n";
        for (auto e: client.game_names) {
            std::cout << e << '\n';
        }
        std::cout << '\n';

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        io_context.poll();

        client.add_game("My game");

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        io_context.poll();


        std::cout << "List of games:\n";
        for (auto e: client.game_names) {
            std::cout << e << '\n';
        }
        std::cout << '\n';
        client.join_game("My game");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        io_context.poll();
        client.make_move(1, 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        io_context.poll();
        client.reverse_token(1, 0);



        // test commands end
        // needs a cycle with sleep and poll, to make some commands, I think.
        while (!io_context.stopped()) {

            //commands

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            io_context.poll();
        }
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }


    return 0;
}