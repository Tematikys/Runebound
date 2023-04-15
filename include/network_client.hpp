#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>
#include <utility>
#include "game_client.hpp"

using boost::asio::ip::tcp;
using json = nlohmann::json;

namespace runebound::network {
    class Client {
    public:
        Client(
                boost::asio::io_context &io_context,
                const std::string &host,
                int port,
                std::string user_name
        )
                : socket_(io_context), m_user_name(std::move(user_name)), io_context_(io_context) {
            tcp::resolver resolver(io_context);
            auto endpoints =
                    tcp::endpoint(boost::asio::ip::address::from_string(host), port);
            socket_.async_connect(endpoints, [this](boost::system::error_code ec) {
                if (!ec) {
                    do_read();
                } else {
                    std::cerr << "Connect failed: " << ec.message() << std::endl;
                }
            });
        }

        void parse_message(std::string &message) {
            json answer = json::parse(message);

            if (answer["change type"] == "game names") {
                game_names = answer["game names"];
            }

            if (answer["change type"] == "game") {
                std::cout << "Game changed, maybe\n";
                runebound::game::from_json(answer, m_game_client);
            }
            if (answer["change type"] == "exception") {
                std::cout << "Eception: " << answer["exception"] << "\n";
            }
        }

        void do_read() {
            boost::asio::async_read_until(
                    socket_, m_buffer, '\n',
                    [this](boost::system::error_code ec, std::size_t length) {
                        if (!ec) {
                            std::istream is(&m_buffer);
                            std::string message;
                            std::getline(is, message);
                            std::cout << "Received: " << message
                                      << " Length: " << length << '\n';
                            parse_message(message);
                            do_read();
                        } else {
                            std::cout << "Disconnected" << std::endl;
                        }
                    }
            );
        }

        void do_write(const std::string &str) {
            socket_.async_write_some(
                    boost::asio::buffer(str + '\n'),
                    [this, str](boost::system::error_code ec, std::size_t length) {
                        if (!ec) {
                            std::cout << "Sent:" << str << ' ' << length << '\n';
                        } else {
                            std::cerr << "Write failed: " << ec.message() << std::endl;
                            socket_.close();
                        }
                    }
            );
        }

        void take_token() {
            json data;
            data["action type"] = "take token";
            do_write(data.dump());
        }

        void add_game(const std::string &game_name) {
            json data;
            data["action type"] = "add game";
            data["game name"] = game_name;
            do_write(data.dump());
        }

        void join_game(const std::string &game_name) {
            json data;
            data["action type"] = "join game";
            data["game name"] = game_name;
            data["user name"] = m_user_name;
            do_write(data.dump());
        }

        void select_character(runebound::character::StandardCharacter character) {
            json data;
            data["action type"] = "select character";
            data["character"] = character;
            do_write(data.dump());
        }


        void make_move(int x, int y) {
            json data;
            data["action type"] = "make move";
            data["x"] = x;
            data["y"] = y;
            do_write(data.dump());
        }

        [[nodiscard]] const std::vector<std::string> &get_game_names() const {
            return game_names;
        }

        [[nodiscard]] int get_game_names_size() const {
            return game_names.size();
        }

        [[nodiscard]] const runebound::game::GameClient &get_game_client() const {
            return m_game_client;
        }


        void exit() {
            io_context_.stop();
        };


    public:
        std::string m_user_name;
        std::vector<std::string> game_names;
        runebound::game::GameClient m_game_client;


    private:
        boost::asio::streambuf m_buffer;
        tcp::socket socket_;
        boost::asio::io_context &io_context_;
    };
}  // namespace runebound::network
#endif  // CLIENT_HPP_