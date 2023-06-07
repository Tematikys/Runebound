#ifndef FIGHT_COMMANDS_HPP_
#define FIGHT_COMMANDS_HPP_

#include <iostream>
#include <vector>
#include "fight.hpp"
#include "runebound_fwd.hpp"

namespace runebound::tests {
void print(const std::vector<runebound::fight::TokenHandCount> tokens) {
    for (auto token : tokens) {
        switch (token.hand) {
            case (runebound::fight::HandFightTokens::MAGICAL_DAMAGE): {
                std::cout << "MAGICAL";
                break;
            }
            case (runebound::fight::HandFightTokens::PHYSICAL_DAMAGE): {
                std::cout << "PHYSICAL";
                break;
            }
            case (runebound::fight::HandFightTokens::ENEMY_DAMAGE): {
                std::cout << "ENEMY";
                break;
            }
            case (runebound::fight::HandFightTokens::DOUBLING): {
                std::cout << "DOUBLING";
                break;
            }
            case (runebound::fight::HandFightTokens::DEXTERITY): {
                std::cout << "DEXTERITY";
                break;
            }
            case (runebound::fight::HandFightTokens::SHIELD): {
                std::cout << "SHIELD";
                break;
            }
            case (runebound::fight::HandFightTokens::NOTHING): {
                std::cout << "NOTHING";
                break;
            }
        }
        std::cout << ' ' << token.count << '\n';
    }
    std::cout << '\n';
}
}  // namespace runebound::tests

#endif  // FIGHT_COMMANDS_HPP_