#include "card_research.hpp"

namespace runebound {
namespace cards {

bool CardResearch::check_hand_dice(
    const ::runebound::map::TypeCell &necessary_result,
    const ::runebound::dice::HandDice &dice
) const {
    switch (necessary_result) {
        case (::runebound::map::TypeCell::WATER):
            return dice == ::runebound::dice::HandDice::MOUNTAINS_WATER ||
                   dice == ::runebound::dice::HandDice::JOKER;
        case (::runebound::map::TypeCell::FOREST):
            return dice == ::runebound::dice::HandDice::FOREST_HILLS ||
                   dice == ::runebound::dice::HandDice::PLAIN_FOREST ||
                   dice == ::runebound::dice::HandDice::JOKER;
        case (::runebound::map::TypeCell::MOUNTAINS):
            return dice == ::runebound::dice::HandDice::MOUNTAINS_WATER ||
                   dice == ::runebound::dice::HandDice::JOKER;
        case (::runebound::map::TypeCell::HILLS):
            return dice == ::runebound::dice::HandDice::FOREST_HILLS ||
                   dice == ::runebound::dice::HandDice::HILLS_PLAIN ||
                   dice == ::runebound::dice::HandDice::JOKER;
        case (::runebound::map::TypeCell::PLAIN):
            return dice == ::runebound::dice::HandDice::PLAIN ||
                   dice == ::runebound::dice::HandDice::PLAIN_FOREST ||
                   dice == ::runebound::dice::HandDice::HILLS_PLAIN ||
                   dice == ::runebound::dice::HandDice::JOKER;
        default:
            return dice == ::runebound::dice::HandDice::JOKER;
    }
}

bool CardResearch::check_completion_task(int x, int y) {
    if (m_task_position_x == x && m_task_position_y == y) {
        m_completed = true;
    }
    return m_completed;
}

bool CardResearch::check_outcome(
    int index,
    std::vector<::runebound::dice::HandDice> &result_dice,
    int &delta_gold,
    int &delta_health
) {
    do {
        bool completed = true;
        for (std::size_t i = 0;
             i < std::min(
                     result_dice.size(),
                     m_outcomes[index].get_size_necessary_result()
                 );
             ++i) {
            if (!check_hand_dice(
                    m_outcomes[index].get_necessary_result(i), result_dice[i]
                )) {
                completed = false;
                break;
            }
        }
        if (completed) {
            delta_health += m_outcomes[index].get_delta_health();
            delta_gold += m_outcomes[index].get_delta_gold();
            return true;
        }
    } while (std::next_permutation(result_dice.begin(), result_dice.end()));
    return false;
}
}  // namespace cards
}  // namespace runebound