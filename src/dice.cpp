#include "dice.hpp"
#include <chrono>
#include <random>

namespace runebound {
namespace dice {
std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());

HandDice throw_dice() {
    int result = rng() % 6;
    return static_cast<HandDice>(result);
}
}  // namespace dice
}  // namespace runebound