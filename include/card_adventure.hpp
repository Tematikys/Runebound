#ifndef CARD_ADVENTURE_HPP_
#define CARD_ADVENTURE_HPP_
#include "nlohmann/json_fwd.hpp"
#include "runebound_fwd.hpp"

namespace runebound::cards {

struct CardAdventure {
    CardAdventure(const CardAdventure &) = delete;
    CardAdventure(CardAdventure &&) = delete;
    CardAdventure &operator=(const CardAdventure &) = delete;
    CardAdventure &operator=(CardAdventure &&) = delete;
    CardAdventure() = default;

};

}  // namespace runebound::cards

#endif  // CARD_ADVENTURE_HPP_
