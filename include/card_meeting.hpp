#ifndef CARD_MEETING_HPP_
#define CARD_MEETING_HPP_

#include <string>
#include "card_adventure.hpp"
#include "nlohmann/json.hpp"
#include "runebound_fwd.hpp"

namespace runebound::cards {

void to_json(nlohmann::json &json, const CardMeeting &card);
void from_json(const nlohmann::json &json, CardMeeting &card);

enum class OptionMeeting { FIRST, SECOND };

void to_json(nlohmann::json &json, const Meeting &meeting);
void from_json(const nlohmann::json &json, Meeting &meeting);

struct Meeting {
private:
    int gold = 0;
    int knowledge_token = 0;
    Characteristic characteristic = Characteristic::BODY;
    int change_characteristic = 0;
    friend struct CardMeeting;

public:
    Meeting() = default;

    Meeting(
        int gold,
        Characteristic characteristic,
        int change_characteristic,
        int knowledge_token
    )
        : gold(gold),
          knowledge_token(knowledge_token),
          characteristic(characteristic),
          change_characteristic(change_characteristic) {
    }

    friend void to_json(nlohmann::json &json, const Meeting &meeting) {
        json["gold"] = meeting.gold;
        json["knowledge_token"] = meeting.knowledge_token;
        json["characteristic"] = meeting.characteristic;
        json["change_characteristic"] = meeting.change_characteristic;
    }

    friend void from_json(const nlohmann::json &json, Meeting &meeting) {
        meeting.change_characteristic = json["change_characteristic"];
        meeting.characteristic = json["characteristic"];
        meeting.gold = json["gold"];
        meeting.knowledge_token = json["knowledge_token"];
    }
};

struct CardMeeting : CardAdventure {
private:
    std::string m_card_name;
    Meeting m_first_option;
    Meeting m_second_option;

public:
    CardMeeting() = default;

    CardMeeting(
        std::string name,
        Characteristic characteristic1,
        int gold1,
        int change1,
        int token1,
        Characteristic characteristic2,
        int gold2,
        int change2,
        int token2
    )
        : m_card_name(std::move(name)),
          m_first_option(Meeting(gold1, characteristic1, change1, token1)),
          m_second_option(Meeting(gold2, characteristic2, change2, token2)) {
    }

    ::runebound::AdventureType m_card_type =
        ::runebound::AdventureType::MEETING;

    [[nodiscard]] int get_gold_award(OptionMeeting option) const {
        if (option == OptionMeeting::FIRST) {
            return m_first_option.gold;
        }
        return m_second_option.gold;
    }

    [[nodiscard]] int get_knowledge_token(OptionMeeting option) const {
        if (option == OptionMeeting::FIRST) {
            return m_first_option.knowledge_token;
        }
        return m_second_option.knowledge_token;
    }

    [[nodiscard]] std::string get_name() const {
        return m_card_name;
    }

    [[nodiscard]] Characteristic get_verifiable_characteristic(
        OptionMeeting option
    ) const {
        if (option == OptionMeeting::FIRST) {
            return m_first_option.characteristic;
        }
        return m_second_option.characteristic;
    }

    [[nodiscard]] int get_change_characteristic(OptionMeeting option) const {
        if (option == OptionMeeting::FIRST) {
            return m_first_option.change_characteristic;
        }
        return m_second_option.change_characteristic;
    }

    friend void to_json(nlohmann::json &json, const CardMeeting &card);
    friend void from_json(const nlohmann::json &json, CardMeeting &card);
};
}  // namespace runebound::cards

#endif  // CARD_MEETING_HPP_