#include <iostream>
#include <boost/asio.hpp>

#include <deque>
#include <set>
#include <memory>
#include <map>
#include <nlohmann/json.hpp>
#include "game.hpp"


using boost::asio::ip::tcp;
using json = nlohmann::json;


class Session;

std::map<std::string, runebound::game::Game> games;
std::map<std::string, std::set<std::string>> game_users;
std::map<std::string, ::runebound::character::Character *> user_character;
std::vector<std::string> game_names;
std::set<Session *> sessions;

class Session : public std::enable_shared_from_this<Session> {
public:
    Session(tcp::socket socket) : socket_(std::move(socket)) {}

    void send_games();

    void start() {
        std::cout << "Some connected\n";
        sessions.insert(this);
        do_read();
        send_games();
    }

    void write(const std::string &message) {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(message),
                                 [this, self, message](boost::system::error_code ec, std::size_t length) {
                                     if (!ec) {
                                         std::cout << "Sent:{ \n " << message << "\n}\n";
                                     } else {
                                         sessions.erase(this);
                                         std::cout << "Disconnected" << std::endl;
                                     }
                                 });
    }

    tcp::socket &socket() { return socket_; }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_, 1024),
                                [this, self](boost::system::error_code ec, std::size_t length) {
                                    if (!ec) {
                                        std::string_view message(data_, length);
                                        std::cout << "Received: " << message;
                                        json data = json::parse(message);
                                        if (data["action type"] == "reverse token") {
                                            std::string game_name = data["game name"];
                                            auto game = games[game_name];
                                            int row = data["x"], column = data["y"];
                                            std::cout << row << ' ' << column << '\n';
                                            std::cout << static_cast<int>(game.m_map.get_cell_map(
                                                    runebound::Point(row, column)).get_side_token()) << '\n';
                                            game.reverse_token(data["x"], data["y"]);
                                            std::cout << static_cast<int>(game.m_map.get_cell_map(
                                                    runebound::Point(row, column)).get_side_token()) << '\n';

                                        }
                                        if (data["action type"] == "add game") {
                                            std::string game_name = data["game name"];
                                            game_names.push_back(game_name);
                                            games[game_name] = runebound::game::Game();
                                            for (auto session: sessions) {
                                                session->send_games();
                                            }
                                        }
                                        if (data["action type"] == "join game") {
                                            std::string game_name = data["game name"];
                                            std::string user_name = data["user name"];

                                            game_ = &games[game_name];
                                            game_users[game_name].insert(user_name);
                                            user_character[user_name] = game_->make_character(0, 0, {0, 0}, 0, 0,
                                                                                              "zero");
                                        }
                                        do_read();
                                    } else {
                                        sessions.erase(this);
                                        std::cout << "Disconnected" << std::endl;
                                    }
                                });
    }


    runebound::game::Game *game_ = nullptr;
    tcp::socket socket_;
    char data_[1024];
};

void Session::send_games() {
    json answer;
    answer["change type"] = "game names";
    answer["game names"] = game_names;
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
                        std::make_shared<Session>(std::move(socket))->start();
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