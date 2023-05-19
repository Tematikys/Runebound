#include "dice.hpp"
#include <vector>
#include "map_cell.hpp"
#include "point.hpp"

namespace runebound {
namespace dice {

HandDice throw_dice() {
    unsigned int result = ::runebound::rng() % 6;
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

bool check_hand_dice(
    ::runebound::map::TypeCell type,
    ::runebound::dice::HandDice dice
) {
    switch (type) {
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
        case (::runebound::map::TypeCell::TOWN):
            return true;
        default:
            return dice == ::runebound::dice::HandDice::JOKER;
    }
}
}  // namespace dice
}  // namespace runebound