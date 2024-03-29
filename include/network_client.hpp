#ifndef CLIENT_HPP_
#define CLIENT_HPP_

// #define NETWORK_DEBUG_INFO

#include <boost/asio.hpp>
#include <chrono>
#include <iostream>
#include <nlohmann/json.hpp>
#include <thread>
#include <utility>
#include "fight_client.hpp"
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
        : socket_(io_context),
          m_user_name(std::move(user_name)),
          io_context_(io_context) {
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

private:
    void parse_message(std::string &message) {
        json answer = json::parse(message);
        if (answer["change type"] == "game names") {
            game_names = answer["game names"];
        }
        if (answer["change type"] == "game") {
#ifdef NETWORK_DEBUG_INFO
            std::cout << "Game changed, maybe\n";
#endif
            runebound::game::from_json(answer, m_game_client);
        }
        if (answer["change type"] == "exception") {
            std::cout << "Exception: " << answer["exception"] << "\n";
        }
        if (answer["change type"] == "selected character") {
            m_character = answer["character"];
        }
        game_need_update = true;
    }

    void do_read() {
        boost::asio::async_read_until(
            socket_, m_buffer, '\n',
            [this](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::istream is(&m_buffer);
                    std::string message;
                    std::getline(is, message);
#ifdef NETWORK_DEBUG_INFO
                    std::cout << "Received: " << message.substr(0, 80)
                              << " Length: " << length << '\n';
#endif
                    parse_message(message);
                    do_read();
                } else {
                    std::cout << "Disconnected" << std::endl;
                }
            }
        );
    }

    void do_write(const std::string &message) {
        socket_.async_write_some(
            boost::asio::buffer(message + '\n'),
            [this, message](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
#ifdef NETWORK_DEBUG_INFO
                    std::cout << "Sent:" << message.substr(0, 80) << ' '
                              << length << '\n';
#endif

                } else {
                    std::cerr << "Write failed: " << ec.message() << std::endl;
                    socket_.close();
                }
            }
        );
    }

public:
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

    void select_free_character(runebound::character::StandardCharacter character
    ) {
        json data;
        data["action type"] = "select free character";
        data["character"] = character;
        do_write(data.dump());
    }

    void throw_move_dice() {
        json data;
        data["action type"] = "throw move dice";
        do_write(data.dump());
    }

    void make_move(int x, int y) {
        json data;
        data["action type"] = "make move";
        data["x"] = x;
        data["y"] = y;
        do_write(data.dump());
    }

    void exit_game() {
        json data;
        data["action type"] = "exit_game";
        do_write(data.dump());
    }

    void exit_game_and_replace_with_bot() {
        json data;
        data["action type"] = "exit_game_and_replace_with_bot";
        do_write(data.dump());
    }

    void relax() {
        json data;
        data["action type"] = "relax";
        do_write(data.dump());
    }

    void pass() {
        json data;
        data["action type"] = "pass";
        do_write(data.dump());
    }

    void fight_end_fight() {
        json data;
        data["action type"] = "fight";
        data["fight command"] = "end fight";
        do_write(data.dump());
    }

    void fight_make(
        runebound::fight::Participant participant,
        std::vector<runebound::fight::TokenHandCount> &tokens_me,
        std::vector<runebound::fight::TokenHandCount> &tokens_enemy
    ) {
        json data;
        data["action type"] = "fight";
        data["fight command"] = "use tokens";
        data["token type"] = "undefined";
        data["participant"] = participant;
        data["tokens_me"] = tokens_me;
        data["tokens_enemy"] = tokens_enemy;
        do_write(data.dump());
    }

    void fight_pass(runebound::fight::Participant participant) {
        json data;
        data["action type"] = "fight";
        data["fight command"] = "fight_pass";
        data["participant"] = participant;
        do_write(data.dump());
    }

    void
    start_card_execution(unsigned int card, runebound::AdventureType type) {
        json data;
        data["action type"] = "adventure";
        data["adventure command"] = "start_card_execution";
        data["card"] = card;
        data["type"] = type;
        do_write(data.dump());
    }

    void throw_research_dice() {
        json data;
        data["action type"] = "adventure";
        data["adventure command"] = "throw_research_dice";
        do_write(data.dump());
    }

    void complete_card_research(std::size_t outcome) {
        json data;
        data["action type"] = "adventure";
        data["adventure command"] = "complete_card_research";
        data["outcome"] = outcome;
        do_write(data.dump());
    }

    void check_characteristic(unsigned int card, cards::OptionMeeting option) {
        json data;
        data["action type"] = "adventure";
        data["adventure command"] = "check_characteristic";
        data["card"] = card;
        data["option"] = option;
        do_write(data.dump());
    }

    void start_trade() {
        json data;
        data["action type"] = "trade";
        data["trade command"] = "start_trade";
        do_write(data.dump());
    }

    void sell_product_in_town(unsigned int product) {
        json data;
        data["action type"] = "trade";
        data["trade command"] = "sell_product_in_town";
        data["product"] = product;
        do_write(data.dump());
    }

    void buy_product(unsigned int product) {
        json data;
        data["action type"] = "trade";
        data["trade command"] = "buy_product";
        data["product"] = product;
        do_write(data.dump());
    }

    void sell_product_in_special_cell(unsigned int product) {
        json data;
        data["action type"] = "trade";
        data["trade command"] = "sell_product_in_special_cell";
        data["product"] = product;
        do_write(data.dump());
    }

    void discard_product(unsigned int product) {
        json data;
        data["action type"] = "trade";
        data["trade command"] = "discard_product";
        data["product"] = product;
        do_write(data.dump());
    }

    void add_bot() {
        json data;
        data["action type"] = "add_bot";
        do_write(data.dump());
    }

    [[nodiscard]] std::vector<dice::HandDice> get_last_dice_result() const {
        return m_game_client.m_last_dice_movement_result;
    };

    [[nodiscard]] const character::Character *get_yourself_character() const {
        if (m_character == runebound::character::StandardCharacter::NONE) {
            std::cout << "Character is not selected, yet\n";
            return nullptr;
        }
        for (auto &character : m_game_client.m_characters) {
            if (character.get_standard_character() == m_character) {
                return &character;
            }
        }
        std::cout << "Something with get_your_character is really wrong\n";
        return nullptr;
    }

    [[nodiscard]] const std::vector<std::string> &get_game_names() const {
        return game_names;
    }

    [[nodiscard]] std::size_t get_game_names_size() const {
        return game_names.size();
    }

    [[nodiscard]] const runebound::game::GameClient &get_game_client() const {
        return m_game_client;
    }

    [[nodiscard]] auto get_winner() const {
        return m_game_client.m_fight_client.get_winner();
    }

    [[nodiscard]] trade::Product get_product(unsigned int index) {
        return m_game_client.m_all_products[index];
    }

    [[nodiscard]] std::vector<Point> get_possible_moves() const {
        return m_game_client.m_possible_moves;
    }

    [[nodiscard]] unsigned int get_active_character_action_points() const {
        if (m_game_client.m_count_players) {
            return m_game_client.m_characters[m_game_client.m_turn]
                .get_action_points();
        } else
            return 0;
    }

    [[nodiscard]] bool is_game_need_update() const {
        bool tmp = game_need_update;
        // game_need_update = false;
        return tmp;
    }

    void exit() {
        io_context_.stop();
    };

public:
    bool game_need_update = true;
    std::string m_user_name;
    runebound::character::StandardCharacter m_character =
        character::StandardCharacter::NONE;
    std::vector<std::string> game_names;
    runebound::game::GameClient m_game_client;

private:
    boost::asio::streambuf m_buffer;
    tcp::socket socket_;
    boost::asio::io_context &io_context_;
};
}  // namespace runebound::network
#endif  // CLIENT_HPP_