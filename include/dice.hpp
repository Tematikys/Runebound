#ifndef DICE_HPP_
#define DICE_HPP_


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

}  // namespace dice
}  // namespace runebound
#endif  // DICE_HPP_
