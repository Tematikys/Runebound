#ifndef CARD_ADVENTURE_HPP_
#define CARD_ADVENTURE_HPP_

namespace runebound::cards {
struct CardAdventure {
    unsigned int height();
    unsigned int width();
    CardAdventure(const CardAdventure &) = delete;
    CardAdventure(CardAdventure &&) = delete;
    CardAdventure &operator=(const CardAdventure &) = delete;
    CardAdventure &operator=(CardAdventure &&) = delete;

protected:
    CardAdventure() = default;

private:
    const unsigned int m_height;
    const double m_ratio;
};

}  // namespace runebound::cards

#endif  // CARD_ADVENTURE_HPP_
