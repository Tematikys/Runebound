#ifndef CARD_ADVENTURE_HPP_
#define CARD_ADVENTURE_HPP_

namespace runebound::cards {
struct CardAdventure {
    CardAdventure(const CardAdventure &) = delete;
    CardAdventure(CardAdventure &&) = delete;
    CardAdventure &operator=(const CardAdventure &) = delete;
    CardAdventure &operator=(CardAdventure &&) = delete;
    CardAdventure() = delete;

private:
    const unsigned int m_height = 1;
    const double m_ratio = 0.66;
};

}  // namespace runebound::cards

#endif  // CARD_ADVENTURE_HPP_
