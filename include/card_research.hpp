#ifndef CARD_RESEARCH_HPP_
#define CARD_RESEARCH_HPP_
#include <algorithm>
#include <map>
#include <vector>
#include "card_adventure.hpp"
#include "dice.hpp"
#include "map_cell.hpp"

namespace runebound {
namespace cards {
struct CardResearch : CardAdventure {
private:
    struct Outcome {
    public:
        int m_delta_gold;
        int m_delta_health;
        std::vector<::runebound::map::TypeCell> m_necessary_result;
        Outcome(
            int delta_gold,
            int delta_health,
            std::vector<runebound::map::TypeCell> necessary_result
        )
            : m_delta_gold(delta_gold),
              m_delta_health(delta_health),
              m_necessary_result(std::move(necessary_result)) {
        }
    };

    int m_task_position_x, m_task_position_y;
    bool m_completed;

    std::vector<Outcome> m_outcomes;

    bool check_hand_dice(
        const ::runebound::map::TypeCell &necessary_result,
        const ::runebound::dice::HandDice &dice
    ) const;

public:
    bool check_completion_task(int x, int y);

    bool check_outcome(
        int index,
        std::vector<::runebound::dice::HandDice> &result_dice,
        int &delta_gold,
        int &delta_health
    );
};
}  // namespace cards
}  // namespace runebound
#endif  // CARD_RESEARCH_HPP_
