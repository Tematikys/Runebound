#include "network_client.hpp"
#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

using boost::asio::ip::tcp;
using json = nlohmann::json;

int main() {
    std::string user_name;
    std::cout << "Write your name:\n";
    std::cin >> user_name;

    boost::asio::io_context io_context;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
        work_guard = boost::asio::make_work_guard(io_context);
    runebound::network::Client client(io_context, "127.0.0.1", 4444, user_name);
    try {
        // test commands begin
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        io_context.poll();

        std::cout << "List of games:\n";
        for (auto e : client.game_names) {
            std::cout << e << '\n';
        }
        std::cout << '\n';

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        io_context.poll();

        client.add_game("My game");

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        io_context.poll();

        std::cout << "List of games:\n";
        for (auto e : client.game_names) {
            std::cout << e << '\n';
        }
        std::cout << '\n';
        client.join_game("My game");
        io_context.poll();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        client.select_character(runebound::character::StandardCharacter::CORBIN
        );

        client.make_move(1, 0);
        io_context.poll();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

        client.take_token();

        int counter = 0;
        while (!io_context.stopped()) {
            std::cout << counter;
            if (counter == 100) {
                //                return 0;
                client.exit();
            } else {
                counter++;
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            io_context.poll();
        }
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}