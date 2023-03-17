#include "dice.hpp"

namespace runebound {
namespace dice {

HandDice throw_dice() {
    int result = ::runebound::rng() % 6;
    return static_cast<HandDice>(result);
}
}  // namespace dice
}  // namespace runebound