#include "character.hpp"

namespace runebound {
namespace character {
std::unique_ptr<::runebound::cards::CardAdventure> Character::pop_card(
    ::runebound::cards::CardAdventure *card
) {
    for (std::size_t i = 0; i < m_cards.size(); ++i) {
        if (m_cards[i].get() == card) {
            std::swap(m_cards[i], m_cards.back());
            break;
        }
    }
    auto desired_card = std::move(m_cards.back());
    m_cards.pop_back();
    return desired_card;
}
}  // namespace character
}  // namespace runebound
