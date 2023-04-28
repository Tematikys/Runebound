#ifndef CARD_MEETING_HPP_
#define CARD_MEETING_HPP_

#include "runebound_fwd.hpp"
#include "card_adventure.hpp"
#include "nlohmann/json_fwd.hpp"

namespace runebound::cards {

void to_json(nlohmann::json &json, const CardMeeting &card);
void from_json(const nlohmann::json &json, CardMeeting &card);

enum class OptionMeeting { FIRST, SECOND };

struct CardMeeting : CardAdventure {
private:
    std::pair <Characteristic, int> m_first_option;
    std::pair <Characteristic, int> m_second_option;
public:
    CardMeeting(Characteristic characteristic1, int gold1,
                Characteristic characteristic2, int gold2) :
          m_first_option({characteristic1, gold1}), m_second_option({characteristic2, gold2}) {}

    ::runebound::AdventureType m_card_type = ::runebound::AdventureType::MEETING;

    [[nodiscard]] int get_gold_award(OptionMeeting option) const {
        if (option == OptionMeeting::FIRST) {
            return m_first_option.second;
        }
        return m_second_option.second;
    }

    [[nodiscard]] Characteristic get_verifiable_characteristic(OptionMeeting option) {
        if (option == OptionMeeting::FIRST) {
            return m_first_option.first;
        }
        return m_second_option.first;
    }

    friend void to_json(nlohmann::json &json, const CardMeeting &card);
    friend void from_json(const nlohmann::json &json, CardMeeting &card);


};
} // namespace runebound::cards

#endif // CARD_MEETING_HPP_