
#ifndef NETWORK_SERVER_HPP
#define NETWORK_SERVER_HPP

#include <boost/asio.hpp>
#include "character.hpp"
#include "runebound_fwd.hpp"

using boost::asio::ip::tcp;

class Connection : public std::enable_shared_from_this<Connection> {
public:
    explicit Connection(tcp::socket socket) : socket_(std::move(socket)){};
    void start();

private:
    void send_game_names();
    void send_selected_character(
        runebound::character::StandardCharacter character
    );
    void send_game_for_all();

    void write(const std::string &message);
    void parse_message(std::string &message);
    void do_read();
    void play_as_bot();
    boost::asio::streambuf m_buffer;
    std::string m_user_name;
    std::string m_game_name;
    runebound::game::Game *m_game = nullptr;
    tcp::socket socket_;
};

#endif  // NETWORK_SERVER_HPP
