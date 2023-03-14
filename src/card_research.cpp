#include "card_research.hpp"
#include <json.hpp>

namespace runebound {
namespace cards {

bool CardResearch::check_hand_dice(
    const ::runebound::map::TypeCell &necessary_result,
    const ::runebound::dice::HandDice &dice
) const {
    switch (necessary_result) {
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
        default:
            return dice == ::runebound::dice::HandDice::JOKER;
    }
}

bool CardResearch::check_completion_task(int x, int y) {
    if (m_task_position_x == x && m_task_position_y == y) {
        m_completed = true;
    }
    return m_completed;
}

bool CardResearch::check_outcome(
    int index,
    std::vector<::runebound::dice::HandDice> &result_dice,
    int &delta_gold,
    int &delta_health
) {
    do {
        bool completed = true;
        for (std::size_t i = 0;
             i < std::min(
                     result_dice.size(),
                     m_outcomes[index].m_necessary_result.size()
                 );
             ++i) {
            if (!check_hand_dice(
                    m_outcomes[index].m_necessary_result[i], result_dice[i]
                )) {
                completed = false;
                break;
            }
        }
        if (completed) {
            delta_health += m_outcomes[index].m_delta_health;
            delta_gold += m_outcomes[index].m_delta_gold;
            return true;
        }
    } while (std::next_permutation(result_dice.begin(), result_dice.end()));
    return false;
}

nlohmann::json CardResearch::to_json() {
    nlohmann::json json;
    json["m_task_position_x"] = m_task_position_x;
    json["m_task_position_y"] = m_task_position_y;
    json["m_completed"] = m_completed;
    std::vector <nlohmann::json> json_outcome(m_outcomes.size());
    for (std::size_t outcome = 0; outcome < m_outcomes.size(); ++outcome) {
        json_outcome[outcome]["m_delta_gold"] = m_outcomes[outcome].m_delta_gold;
        json_outcome[outcome]["m_delta_health"] = m_outcomes[outcome].m_delta_health;
        json_outcome[outcome]["m_necessary_result"] = m_outcomes[outcome].m_necessary_result;
    }
    json["m_outcomes"] = json_outcome;
    return json;
}

void to_json(nlohmann::json &json, const CardResearch& card) {
    json["m_task_position_x"] = card.m_task_position_x;
    json["m_task_position_y"] = card.m_task_position_y;
    json["m_completed"] = card.m_completed;
    std::vector <nlohmann::json> json_outcome(card.m_outcomes.size());
    for (std::size_t outcome = 0; outcome < card.m_outcomes.size(); ++outcome) {
        json_outcome[outcome]["m_delta_gold"] = card.m_outcomes[outcome].m_delta_gold;
        json_outcome[outcome]["m_delta_health"] = card.m_outcomes[outcome].m_delta_health;
        json_outcome[outcome]["m_necessary_result"] = card.m_outcomes[outcome].m_necessary_result;
    }
    json["m_outcomes"] = json_outcome;
}

void from_json(const nlohmann::json &json, CardResearch& card) {
    card.m_task_position_x = json["m_task_position_x"];
    card.m_task_position_y = json["m_task_position_y"];
    card.m_completed = json["m_completed"];
    std::vector <nlohmann::json> json_outcome = json["m_outcomes"];
    card.m_outcomes.resize(json_outcome.size());
    for (std::size_t outcome = 0; outcome < json_outcome.size(); ++outcome) {
        card.m_outcomes[outcome].m_delta_gold = json_outcome[outcome]["m_delta_gold"];
        card.m_outcomes[outcome].m_delta_health = json_outcome[outcome]["m_delta_health"];
        card.m_outcomes[outcome].m_necessary_result.resize(json_outcome[outcome]["m_necessary_result"].size());
        for (std::size_t i = 0; i < json_outcome[outcome]["m_necessary_result"].size(); ++i) {
            card.m_outcomes[outcome].m_necessary_result[i] = json_outcome[outcome]["m_necessary_result"][i];
        }
    }
}
}  // namespace cards
}  // namespace runebound
