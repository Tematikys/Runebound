#include "game.hpp"

namespace runebound {
namespace game {
void Game::make_move(
    const ::runebound::character::Character *chr,
    int end_x,
    int end_y,
    std::vector<::runebound::dice::HandDice> &dice_roll_results
) {
    if (chr != &m_characters[m_turn]) {
        return;
    }
    if (!m_map.check_move(
            m_characters[m_turn].get_current_x(),
            m_characters[m_turn].get_current_x(), end_x, end_y,
            dice_roll_results
        )) {
        return;
    }
    m_characters[m_turn].set_current_x(end_x);
    m_characters[m_turn].set_current_y(end_y);
    m_turn = (m_turn + 1) % m_count_players;
}

}  // namespace game
}  // namespace runebound