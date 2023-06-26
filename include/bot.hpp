#ifndef BOT_HPP_
#define BOT_HPP_

#include "game.hpp"
#include "network_server.hpp"
#include "nlohmann/json_fwd.hpp"
#include "runebound_fwd.hpp"

namespace runebound::bot {

struct Bot {
private:
    std::shared_ptr<character::Character> m_bot;
    game::Game *m_game;
    Connection *m_connection;
    boost::asio::steady_timer  m_timer;

    void start_characteristic_check() {
        m_game->check_characteristic(
            m_bot, *(m_bot->get_cards(AdventureType::MEETING).begin()),
            cards::OptionMeeting::FIRST
        );
        m_connection->send_game_for_all();
    }

    void throw_dice() {
        if (m_bot->get_action_points() == 0) {
            m_game->start_next_character_turn(m_bot);
            return;
        }
        if (m_bot->get_action_points() >= 2 &&
            m_game->get_map()
                    .get_cell_map(m_bot->get_position())
                    .get_side_token() == Side::FRONT &&
            m_game->get_map().get_cell_map(m_bot->get_position()).get_token() ==
                AdventureType::MEETING) {
            m_game->take_token(m_bot);
            m_connection->send_game_for_all();
            m_game->start_next_character_turn(m_bot);
            m_timer.async_wait(std::bind(&Bot::start_characteristic_check, this)
            );
            return;
        }
        m_game->throw_movement_dice(m_bot);
        m_connection->send_game_for_all();
        m_timer.async_wait(std::bind(&Bot::make_move, this));
    }

    void make_move() {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        auto possible_moves = m_game->get_possible_moves();
        m_game->make_move(
            m_bot, possible_moves[runebound::rng() % possible_moves.size()]
        );
        m_connection->send_game_for_all();
        m_timer.async_wait(std::bind(&Bot::throw_dice, this));
    }

public:
    Bot(game::Game *game,
        Connection *connection,
        boost::asio::io_context &io_context)
        : m_game(game), m_connection(connection), m_timer(io_context) {
        m_bot = m_game->get_active_character();
        m_timer.expires_after(std::chrono::milliseconds(1500));
        m_timer.async_wait(std::bind(&Bot::throw_dice, this));
        io_context.run();
    }
};
}  // namespace runebound::bot

#endif  // BOT_HPP_