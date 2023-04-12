#include <iostream>
#include <boost/asio.hpp>

#include <deque>
#include <set>
#include <memory>
#include <map>
#include <nlohmann/json.hpp>
#include "game.hpp"
#include "game_client.hpp"


using boost::asio::ip::tcp;
using json = nlohmann::json;


//user - string с именем connection
class Connection;

std::vector<std::string> game_names;
std::set<Connection *> connections;
std::map<std::string, runebound::game::Game> games;
std::map<std::string, std::set<std::string>> game_users;
std::map<std::string, ::runebound::character::Character *> user_character;
std::map<std::string, Connection *> user_connection;
std::map<std::string, ::runebound::map::MapClient> game_map;

class Connection : public std::enable_shared_from_this<Connection> {
public:
    Connection(tcp::socket socket) : socket_(std::move(socket)) {}

    void send_game_names();

    void send_game();

    void start() {
        std::cout << "Some connected\n";
        connections.insert(this);
        do_read();
        send_game_names();
    }

    void write(const std::string &message) {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(message+'\n'),
                                 [this, self, message](boost::system::error_code ec, std::size_t length) {
                                     if (!ec) {
                                         std::cout << "Sent:{ \n " << message << "\n}\n";
                                     } else {
                                         connections.erase(this);
                                         std::cout << "Disconnected" << std::endl;
                                     }
                                 });
    }

    void parse_message(std::string &message) {
        json data = json::parse(message);
        if (data["action type"] == "reverse token") {
            int x = data["x"], y = data["y"];
            m_game->reverse_token(x, y);
            for (const std::string &user_name: game_users[m_game_name]) {
                user_connection[user_name]->send_game();
            }
        }
        if (data["action type"] == "add game") {
            std::string game_name = data["game name"];
            game_names.push_back(game_name);
            games[game_name] = runebound::game::Game();
            game_map[game_name] = runebound::map::MapClient();
            for (auto session: connections) {
                session->send_game_names();
            }
        }
        if (data["action type"] == "join game") {
            std::string game_name = data["game name"];
            std::string user_name = data["user name"];
            m_user_name = user_name;
            m_game_name = game_name;
            m_game = &games[game_name];
            user_connection[m_user_name] = this;
            game_users[game_name].insert(m_user_name);
            user_character[user_name] = m_game->make_character(0, 0, {0, 0}, 0, 3,
                                                               m_user_name);
            for (const std::string &user_name: game_users[m_game_name]) {
                user_connection[user_name]->send_game();
            }
        }
        if (data["action type"] == "make move") {
            int x = data["x"], y = data["y"];
            auto dice_result = runebound::dice::get_combination_of_dice(
                    user_character[m_user_name]->get_speed());
            m_game->make_move(user_character[m_user_name], {x, y}, dice_result);
            for (const std::string &user_name: game_users[m_game_name]) {
                user_connection[user_name]->send_game();
            }
        }
    }


private:

    void do_read() {
        auto self(shared_from_this());

        boost::asio::async_read_until(socket_, m_buffer, '\n',
                                      [this, self](boost::system::error_code ec, std::size_t length) {
                                          if (!ec) {
                                              std::istream is(&m_buffer);
                                              std::string message;
                                              std::getline(is, message);
                                              std::cout << "Received: " << message << " Length: " << length << '\n';
                                              parse_message(message);
                                              do_read();
                                          } else {
                                              connections.erase(this);
                                              std::cout << "Disconnected" << std::endl;
                                          }
                                      });
    }
    boost::asio::streambuf m_buffer;
    std::string m_user_name;
    std::string m_game_name;
    runebound::game::Game *m_game = nullptr;
    tcp::socket socket_;
};

void Connection::send_game_names() {
    json answer;
    answer["change type"] = "game names";
    answer["game names"] = game_names;
    write(answer.dump());
}

void Connection::send_game() {
    json answer;
    answer["change type"] = "game";
    runebound::map::to_json(answer, game_map[m_game_name]);
    write(answer.dump());
}


class Server {
public:
    Server(boost::asio::io_context &io_context, short port)
            : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        std::cout << "Server started\n";
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
                [this](boost::system::error_code ec, tcp::socket socket) {
                    if (!ec) {
                        std::make_shared<Connection>(std::move(socket))->start();
                    }

                    do_accept();
                });
    }

    tcp::acceptor acceptor_;
};

int main() {
    try {
        boost::asio::io_context io_context;
        Server server(io_context, 4444);
        io_context.run();
    }
    catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}