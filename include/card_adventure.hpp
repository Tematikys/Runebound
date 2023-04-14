#ifndef CARD_ADVENTURE_HPP_
#define CARD_ADVENTURE_HPP_
#include <nlohmann/json_fwd.hpp>
#include "runebound_fwd.hpp"

namespace runebound::cards {

struct CardAdventure {
protected:
    bool m_trophy = false;
    ::runebound::AdventureType m_card_type;
public:
    CardAdventure(const CardAdventure &) = delete;
    CardAdventure(CardAdventure &&) = delete;
    CardAdventure &operator=(const CardAdventure &) = delete;
    CardAdventure &operator=(CardAdventure &&) = delete;
    CardAdventure() = default;
    virtual ~CardAdventure() = default;

    [[nodiscard]] virtual ::runebound::AdventureType get_card_type() {
        return m_card_type;
    }

    [[nodiscard]] virtual bool check_trophy() const {
        return m_trophy;
    }

    void virtual make_trophy() {
        m_trophy = true;
    }
};

}  // namespace runebound::cards

#endif  // CARD_ADVENTURE_HPP_
