#ifndef DICE_HPP_
#define DICE_HPP_

#include <random>
#include <chrono>

namespace runebound {
namespace dice {
std::mt19937 rng(std::chrono::steady_clock::now ().time_since_epoch().count());
enum class HandDice { JOKER, PLAIN, PLAIN_FOREST, FOREST_HILLS, MOUNTAINS_WATER, HILLS_PLAIN};

HandDice throw_dice() {
    int result = rng() % 6;
    return static_cast<HandDice>(result);
}

} // namespace dice
} // namespace runebound
#endif  // DICE_HPP_
