#include "network_client.hpp"
#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>

using boost::asio::ip::tcp;
using json = nlohmann::json;
boost::asio::io_context io_context;

void wait() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    io_context.poll();
}

int main() {
    std::string user_name;
    std::cout << "Write your name:\n";
    std::cin >> user_name;

    boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
        work_guard = boost::asio::make_work_guard(io_context);

    runebound::network::Client client(io_context, "127.0.0.1", 4444, user_name);
    try {
        // test commands begin
        wait();

        std::cout << "List of games:\n";
        for (const auto &e : client.game_names) {
            std::cout << e << '\n';
        }
        std::cout << '\n';

        wait();

        client.add_game("My game");

        wait();

        std::cout << "List of games:\n";
        for (const auto &e : client.game_names) {
            std::cout << e << '\n';
        }
        std::cout << '\n';
        client.join_game("My game");

        wait();

        std::cout << "################";
        for (auto e :
             client.get_game_client().m_remaining_standard_characters) {
            std::cout << static_cast<int>(e);
        }

        wait();
        {
            std::cout<<"Try to get not available character\n";
            auto chr = client.get_yourself_character();
            std::cout << "Character: "
                      << static_cast<int>(chr.get_standard_character()) << '\n';
        }
        client.select_character(runebound::character::StandardCharacter::CORBIN
        );

        wait();
        {
            auto chr = client.get_yourself_character();
            std::cout << "Character: "
                      << static_cast<int>(chr.get_standard_character()) << '\n';
            for (auto e :
                 client.get_game_client().m_remaining_standard_characters) {
                std::cout << static_cast<int>(e);
            }
        }
        wait();

        client.throw_move_dice();

        wait();

        client.make_move(1, 0);

        wait();

        client.take_token();

        wait();

        client.relax();

        wait();

        client.pass();

        wait();

        int counter = 0;
        while (!io_context.stopped()) {
            std::cout << counter;
            if (counter == 50) {
                client.exit_game();
            }

            if (counter == 100) {
                //                return 0;
                client.exit();
            } else {
                counter++;
            }

            wait();
        }
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}