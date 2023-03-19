#include "dice.hpp"
#include <vector>

namespace runebound {
namespace dice {

HandDice throw_dice() {
    int result = ::runebound::rng() % 6;
    return static_cast<HandDice>(result);
}

std::vector<HandDice> get_combination_of_dice(unsigned int count_throws) {
    std::vector<HandDice> result_of_throws(count_throws);
    for (unsigned int number_dice = 0; number_dice < count_throws;
         ++number_dice) {
        result_of_throws[number_dice] = throw_dice();
    }
    return result_of_throws;
}
}  // namespace dice
}  // namespace runebound