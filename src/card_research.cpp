#include "card_research.hpp"
#include <nlohmann/json.hpp>
#include "map.hpp"

namespace runebound {
namespace cards {

bool CardResearch::check_completion_task(const Point &cell) {
    if (m_required_cells.count(cell) > 0) {
        m_completed = true;
    }
    return m_completed;
}

bool CardResearch::check_outcome(
    int index,
    std::vector<::runebound::dice::HandDice> &result_dice
) const {
    do {
        bool completed = true;
        for (std::size_t i = 0;
             i <
             std::min(
                 result_dice.size(), m_outcomes[index].m_necessary_result.size()
             );
             ++i) {
            if (!::runebound::dice::check_hand_dice(
                    m_outcomes[index].m_necessary_result[i], result_dice[i]
                )) {
                completed = false;
                break;
            }
        }
        if (completed) {
            return true;
        }
    } while (std::next_permutation(result_dice.begin(), result_dice.end()));
    return false;
}

void to_json(nlohmann::json &json, const CardResearch &card) {
    json["m_required_territory"] = card.m_required_territory;
    json["m_completed"] = card.m_completed;
    json["m_card_name"] = card.m_card_name;
    json["m_card_type"] = card.m_card_type;
    std::vector<nlohmann::json> json_outcome(card.m_outcomes.size());
    for (std::size_t outcome = 0; outcome < card.m_outcomes.size(); ++outcome) {
        json_outcome[outcome]["m_delta_gold"] =
            card.m_outcomes[outcome].m_delta_gold;
        json_outcome[outcome]["m_delta_health"] =
            card.m_outcomes[outcome].m_delta_health;
        json_outcome[outcome]["m_knowledge_token"] =
            card.m_outcomes[outcome].m_knowledge_token;
        json_outcome[outcome]["m_necessary_result"] =
            card.m_outcomes[outcome].m_necessary_result;
    }
    json["m_outcomes"] = json_outcome;
}

void from_json(const nlohmann::json &json, CardResearch &card, map::Map &map) {
    card.m_required_territory = json["m_required_territory"];
    for (const auto &cell :
         map.get_territory_cells(card.m_required_territory)) {
        card.m_required_cells.insert(cell);
    }
    card.m_completed = json["m_completed"];
    card.m_card_name = json["m_card_name"];
    card.m_card_type = json["m_card_type"];
    std::vector<nlohmann::json> json_outcome = json["m_outcomes"];
    card.m_outcomes.resize(json_outcome.size());
    for (std::size_t outcome = 0; outcome < json_outcome.size(); ++outcome) {
        card.m_outcomes[outcome].m_delta_gold =
            json_outcome[outcome]["m_delta_gold"];
        card.m_outcomes[outcome].m_delta_health =
            json_outcome[outcome]["m_delta_health"];
        card.m_outcomes[outcome].m_knowledge_token =
            json_outcome[outcome]["m_knowledge_token"];
        card.m_outcomes[outcome].m_necessary_result.resize(
            json_outcome[outcome]["m_necessary_result"].size()
        );
        for (std::size_t i = 0;
             i < json_outcome[outcome]["m_necessary_result"].size(); ++i) {
            card.m_outcomes[outcome].m_necessary_result[i] =
                json_outcome[outcome]["m_necessary_result"][i];
        }
    }
}
}  // namespace cards
}  // namespace runebound
