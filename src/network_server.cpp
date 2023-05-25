#include <boost/asio.hpp>
#include <deque>
#include <iostream>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <set>
#include "character.hpp"
#include "fight.hpp"
#include "game.hpp"
#include "game_client.hpp"

// #define NETWORK_DEBUG_INFO

using boost::asio::ip::tcp;
using json = nlohmann::json;

class Connection;

std::vector<std::string> game_names;
std::set<Connection *> connections;
std::map<std::string, runebound::game::Game> games;
std::map<std::string, std::set<std::string>> game_users;
std::map<std::string, std::shared_ptr<runebound::character::Character>>
    user_character;
std::map<std::string, Connection *> user_connection;
int counter = 0;

class Connection : public std::enable_shared_from_this<Connection> {
public:
    explicit Connection(tcp::socket socket) : socket_(std::move(socket)) {
    }

    void send_game_names();
    void send_selected_character(
        runebound::character::StandardCharacter character
    );
    void send_game();

    void start() {
        std::cout << "Some connected\n";
        connections.insert(this);
        do_read();
        send_game_names();
    }

    void write(const std::string &message) {
        auto self(shared_from_this());
        boost::asio::async_write(
            socket_, boost::asio::buffer(message + '\n'),
            [this, self,
             message](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
#ifdef NETWORK_DEBUG_INFO
                    std::cout << "Sent:" << message.substr(0, 80) << ' '
                              << length << std::endl;
#endif
                } else {
                    std::cerr << "Write failed: " << ec.message() << std::endl;
                    connections.erase(this);
                    std::cout << "Disconnected" << std::endl;
                }
            }
        );
    }

    void parse_message(std::string &message) {
        try {
            json data = json::parse(message);

            if (data["action type"] == "take token") {
                m_game->take_token(user_character[m_user_name]);
                for (const std::string &user_name : game_users[m_game_name]) {
                    user_connection[user_name]->send_game();
                }
            }
            if (data["action type"] == "add game") {
                std::string game_name = data["game name"];
                game_names.push_back(game_name);
                games[game_name] = runebound::game::Game();
                for (auto session : connections) {
                    session->send_game_names();
                }
            }

            if (data["action type"] == "join game") {
                m_game_name = data["game name"];
                m_user_name = data["user name"];
                m_user_name += std::to_string(counter++);

                m_game = &games[m_game_name];
                user_connection[m_user_name] = this;
                game_users[m_game_name].insert(m_user_name);

                for (const std::string &user_name : game_users[m_game_name]) {
                    user_connection[user_name]->send_game();
                }
            }

            if (data["action type"] == "exit game") {
                game_users[m_game_name].erase(m_user_name);
                // m_game->delete_character(user_character[m_user_name]);
                user_character[m_user_name] = nullptr;
                m_game = nullptr;
                m_game_name = "";
                send_selected_character(
                    runebound::character::StandardCharacter::NONE
                );
                for (const std::string &user_name : game_users[m_game_name]) {
                    user_connection[user_name]->send_game();
                }
            }

            if (data["action type"] == "select character") {
                runebound::character::StandardCharacter character =
                    data["character"];
                user_character[m_user_name] = m_game->make_character(character);
                if (character ==
                    runebound::character::StandardCharacter::NONE) {
                    send_selected_character(character);
                    for (const std::string &user_name :
                         game_users[m_game_name]) {
                        user_connection[user_name]->send_game();
                    }
                } else {
                    for (const std::string &user_name :
                         game_users[m_game_name]) {
                        user_connection[user_name]->send_game();
                    }
                    send_selected_character(character);
                }
            }
            if (data["action type"] == "throw move dice") {
                m_game->throw_movement_dice(user_character[m_user_name]);
                for (const std::string &user_name : game_users[m_game_name]) {
                    user_connection[user_name]->send_game();
                }
            }
            if (data["action type"] == "make move") {
                int x = data["x"], y = data["y"];
                auto dice = m_game->get_last_dice_movement_result();
                auto path = m_game->make_move(
                    user_character[m_user_name], {x, y}, dice
                );
                for (const std::string &user_name : game_users[m_game_name]) {
                    user_connection[user_name]->send_game();
                }
            }

            if (data["action type"] == "pass") {
                m_game->start_next_character_turn(user_character[m_user_name]);
                for (const std::string &user_name : game_users[m_game_name]) {
                    user_connection[user_name]->send_game();
                }
            }

            if (data["action type"] == "relax") {
                m_game->throw_relax_dice(user_character[m_user_name]);
                m_game->relax(user_character[m_user_name]);
                for (const std::string &user_name : game_users[m_game_name]) {
                    user_connection[user_name]->send_game();
                }
            }

            if (data["action type"] == "fight") {
                if (data["fight command"] == "end fight") {
                    m_game->end_fight(user_character[m_user_name]);
                }

                if (data["fight command"] == "use tokens") {
                    if (data["token type"] == "undefined") {
                        std::vector<runebound::fight::TokenHandCount>
                            tokens_me = data["tokens_me"];
                        std::vector<runebound::fight::TokenHandCount>
                            tokens_enemy = data["tokens_enemy"];
                        runebound::fight::Participant participant_me =
                            data["participant"];
                        runebound::fight::Participant participant_enemy =
                            (participant_me ==
                             runebound::fight::Participant::CHARACTER)
                                ? runebound::fight::Participant::ENEMY
                                : runebound::fight::Participant::CHARACTER;
                        if (tokens_me.empty()) {
                            throw std::runtime_error("0 size");
                        }
                        // Dexterity
                        for (auto token : tokens_me) {
                            if (token.hand ==
                                runebound::fight::HandFightTokens::DEXTERITY) {
                                if ((tokens_enemy.size() == 1) &&
                                    (tokens_me.size() == 1)) {
                                    m_game
                                        ->get_current_fight()
                                        ->make_dexterity(
                                            participant_me, tokens_me[0],
                                            tokens_enemy[0], participant_enemy
                                        );
                                } else {
                                    if ((tokens_enemy.empty()) &&
                                        (tokens_me.size() == 2)) {
                                        if (tokens_me[0].hand ==
                                            runebound::fight::HandFightTokens::
                                                DEXTERITY) {
                                            m_game
                                                ->get_current_fight()
                                                ->make_dexterity(
                                                    participant_me,
                                                    tokens_me[0], tokens_me[1],
                                                    participant_me
                                                );
                                        } else {
                                            m_game
                                                ->get_current_fight()
                                                ->make_dexterity(
                                                    participant_me,
                                                    tokens_me[1], tokens_me[0],
                                                    participant_me
                                                );
                                        }
                                    } else {
                                        throw std::runtime_error(
                                            "Wrong dexterity"
                                        );
                                    }
                                }
                            }
                        }
                        // Doubling
                        for (auto token : tokens_me) {
                            if (token.hand ==
                                runebound::fight::HandFightTokens::DOUBLING) {
                                if ((tokens_enemy.empty()) &&
                                    (tokens_me.size() == 2)) {
                                    if (tokens_me[0].hand ==
                                        runebound::fight::HandFightTokens::
                                            DOUBLING) {
                                        m_game
                                            ->get_current_fight()
                                            ->make_doubling(
                                                participant_me, tokens_me[0],
                                                tokens_me[1]
                                            );
                                    } else {
                                        m_game
                                            ->get_current_fight()
                                            ->make_doubling(
                                                participant_me, tokens_me[1],
                                                tokens_me[0]
                                            );
                                    }
                                } else {
                                    throw std::runtime_error("Wrong doubling");
                                }
                            }
                        }
                        // Damadge
                        if ((tokens_enemy.empty()) &&
                            (tokens_me[0].hand ==
                                 runebound::fight::HandFightTokens::
                                     ENEMY_DAMAGE ||
                             tokens_me[0].hand ==
                                 runebound::fight::HandFightTokens::
                                     MAGICAL_DAMAGE ||
                             tokens_me[0].hand ==
                                 runebound::fight::HandFightTokens::
                                     PHYSICAL_DAMAGE)) {
                            m_game
                                ->get_current_fight()
                                ->make_damage(participant_me, tokens_me);
                        } else {
                            throw std::runtime_error("Wrong damadge");
                        }
                    }

                    if (data["token type"] == "doubling") {
                        runebound::fight::Participant participant =
                            data["participant"];
                        runebound::fight::TokenHandCount token1 =
                            data["token1"];
                        runebound::fight::TokenHandCount token2 =
                            data["token2"];
                        m_game
                            ->get_current_fight()
                            ->make_doubling(participant, token1, token2);
                    }
                    if (data["token type"] == "dexterity") {
                        runebound::fight::Participant participant1 =
                            data["participant1"];
                        runebound::fight::Participant participant2 =
                            data["participant2"];
                        runebound::fight::TokenHandCount token1 =
                            data["token1"];
                        runebound::fight::TokenHandCount token2 =
                            data["token2"];
                        m_game
                            ->get_current_fight()
                            ->make_dexterity(
                                participant1, token1, token2, participant2
                            );
                    }
                    if (data["token type"] == "damage") {
                        runebound::fight::Participant participant =
                            data["participant"];
                        std::vector<runebound::fight::TokenHandCount> tokens =
                            data["tokens"];
                        m_game
                            ->get_current_fight()
                            ->make_damage(participant, tokens);
                    }
                }
                if (data["fight command"] == "fight_pass") {
                    if (data["participant"] ==
                        runebound::fight::Participant::CHARACTER) {
                        m_game
                            ->get_current_fight()
                            ->pass_character();
                    } else {
                        if (data["participant"] ==
                            runebound::fight::Participant::ENEMY) {
                            m_game
                                ->get_current_fight()
                                ->pass_enemy();
                        }
                    }
                }
                for (const std::string &user_name : game_users[m_game_name]) {
                    user_connection[user_name]->send_game();
                }
            }

            if (data["action type"] == "trade") {
                if (data["trade command"] == "start_trade") {
                    m_game->start_trade(user_character[m_user_name]);
                } else if (data["trade command"] == "sell_product_in_town") {
                    m_game->sell_product_in_town(
                        user_character[m_user_name], data["product"]
                    );
                } else if (data["trade command"] == "buy_product") {
                    m_game->buy_product(
                        user_character[m_user_name], data["product"]
                    );
                } else if (data["trade command"] == "sell_product_in_special_cell") {
                    m_game->sell_product_in_special_cell(
                        user_character[m_user_name], data["product"]
                    );
                } else if (data["trade command"] == "discard_product") {
                    m_game->discard_product(
                        user_character[m_user_name], data["product"]
                    );
                }
            }

            if (data["action type"] == "adventure") {
                if (data["adventure command"] == "throw_research_dice") {
                    m_game->throw_research_dice(user_character[m_user_name]);
                }
                if (data["adventure command"] == "complete_card_research") {
                    m_game->complete_card_research(
                        user_character[m_user_name], data["outcome"]
                    );
                }
                if (data["adventure command"] == "check_characteristic") {
                    m_game->check_characteristic(
                        user_character[m_user_name], data["card"],
                        data["option"]
                    );
                }
                for (const std::string &user_name : game_users[m_game_name]) {
                    user_connection[user_name]->send_game();
                }
            }
        } catch (std::exception &e) {
            std::cout << e.what() << '\n';
            json answer;
            answer["change type"] = "exception";
            answer["exception"] = e.what();
            write(answer.dump());
        }
    }

private:
    void do_read() {
        auto self(shared_from_this());

        boost::asio::async_read_until(
            socket_, m_buffer, '\n',
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::istream is(&m_buffer);
                    std::string message;
                    std::getline(is, message);
#ifdef NETWORK_DEBUG_INFO
                    std::cout << "Received: " << message.substr(0, 80) << ' '
                              << length << '\n';
#endif
                    parse_message(message);
                    do_read();
                } else {
                    connections.erase(this);
                    std::cout << "Disconnected" << std::endl;
                }
            }
        );
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

void Connection::send_selected_character(
    runebound::character::StandardCharacter character
) {
    json answer;
    answer["change type"] = "selected character";
    answer["character"] = character;
    write(answer.dump());
}

void Connection::send_game() {
    json answer;
    if (user_character.contains(m_user_name)) {
        auto fight = m_game->get_current_fight();
        if (fight != nullptr) {
            if (fight->check_end_round()) {
                fight->start_round();
            }
        }
    }
    auto game = ::runebound::game::GameClient(*m_game);
    answer["change type"] = "game";
    runebound::game::to_json(answer, game);
    write(answer.dump());
}

class Server {
public:
    Server(boost::asio::io_context &io_context, short port)
        : m_acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
        std::cout << "Server started\n";
        do_accept();
    }

private:
    void do_accept() {
        m_acceptor.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Connection>(std::move(socket))->start();
                }

                do_accept();
            }
        );
    }

    tcp::acceptor m_acceptor;
};

int main() {
    try {
        boost::asio::io_context io_context;
        Server server(io_context, 4444);
        io_context.run();
    } catch (std::exception &e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}