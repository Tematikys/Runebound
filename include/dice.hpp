#ifndef DICE_HPP_
#define DICE_HPP_

#include "map_cell.hpp"
#include "runebound_fwd.hpp"

namespace runebound {
namespace dice {

enum class HandDice {
    JOKER,
    PLAIN,
    PLAIN_FOREST,
    FOREST_HILLS,
    MOUNTAINS_WATER,
    HILLS_PLAIN
};

HandDice throw_dice();
std::vector<HandDice> get_combination_of_dice(unsigned int count_throws);
bool check_hand_dice(
    ::runebound::map::TypeCell type,
    ::runebound::dice::HandDice dice
);

}  // namespace dice
}  // namespace runebound
#endif  // DICE_HPP_
