#ifndef BOT_HPP_
#define BOT_HPP_

#include "runebound_fwd.hpp"
#include "network_client.hpp"
#include "game_client.hpp"
namespace runebound::bot {

enum class BotCommands {MAKE_MOVE, RELAX, THROW_DICE, PASS};
struct Bot {
private:
    ::boost::asio::io_context m_io_context;
    runebound::network::Client m_network_client =
        ::runebound::network::Client(m_io_context, "127.0.0.1", 4444, "client");

    Bot() {
        m_network_client.select_character(*m_network_client.get_game_client().m_remaining_standard_characters.begin());
    }
public:
    void send_command() {
        auto turn = m_network_client.get_game_client().m_characters[m_network_client.get_game_client().m_turn].get_standard_character();
        if (m_network_client.m_character == turn) {
            BotCommands command = static_cast<BotCommands>(rng() % 3);
            if (m_network_client.m_character !=
                ::runebound::character::StandardCharacter::NONE) {
                return;
            }
            switch (command) {
                case (BotCommands::MAKE_MOVE): {
                    auto results_throwing_dice = m_network_client.get_game_client()
                                                     .m_last_dice_movement_result;
                    auto yourself = m_network_client.get_yourself_character();
                    if (results_throwing_dice.size() == 0) {
                        auto position = yourself.get_position();
                        m_network_client.make_move(position.x + rng() % 3 - 1, position.y + rng() % 3 - 1);
                    }
                    else {
                        m_network_client.make_move(rng() % 15, rng() % 15);
                    }
                    break;
                }
                case (BotCommands::RELAX): {
                    m_network_client.relax();
                    break;
                }
                case (BotCommands::THROW_DICE): {
                    m_network_client.throw_move_dice();
                    break;
                }
                case (BotCommands::PASS): {
                    m_network_client.pass();
                    break;
                }
            }
        }
    }
};
} // namespace runebound::bot
#endif // BOT_HPP_