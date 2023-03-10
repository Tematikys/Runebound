#ifndef GAME_HPP_
#define GAME_HPP_

#include <map>
#include <memory>
#include <utility>
#include <vector>
#include "character.hpp"
#include "map.hpp"
#include "tokens.hpp"

namespace runebound {
const int DECK_SIZE = 60;

namespace game {
struct Game {
private:
    ::runebound::map::Map m_map;
    std::vector<::runebound::character::Character> m_characters;
    std::vector<std::unique_ptr<::runebound::cards::CardAdventure>>
        m_cards_adventure;
    std::map<::runebound::token::Token, unsigned int> m_tokens;
    unsigned int m_turn = 0;
    const unsigned int M_COUNT_PLAYERS;

public:
    Game() : M_COUNT_PLAYERS(1) {
        m_cards_adventure.resize(DECK_SIZE);
    };

    explicit Game(unsigned int count_players) : M_COUNT_PLAYERS(count_players) {
        m_characters.resize(count_players);
        m_cards_adventure.resize(DECK_SIZE);
    }

    void make_move(
        const ::runebound::character::Character *chr,
        int end_x,
        int end_y,
        std::vector<::runebound::dice::HandDice> &dice_roll_results
    );
};
}  // namespace game
}  // namespace runebound
#endif  // GAME_HPP_
