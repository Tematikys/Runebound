#ifndef CARD_RESEARCH_HPP_
#define CARD_RESEARCH_HPP_
#include <algorithm>
#include <json_fwd.hpp>
#include <map>
#include <memory>
#include <vector>
#include "card_adventure.hpp"
#include "dice.hpp"
#include "map_cell.hpp"

namespace runebound {
namespace cards {
void to_json(nlohmann::json &json, const CardResearch &card);
void from_json(const nlohmann::json &json, CardResearch &card);

struct CardResearch : CardAdventure {
public:
    struct Outcome;

private:
    int m_task_position_x, m_task_position_y;
    bool m_completed = false;

    std::vector<Outcome> m_outcomes;

    [[nodiscard]] bool check_completion_task(int x, int y);

public:
    ::runebound::AdventureType m_card_type =
        ::runebound::AdventureType::RESEARCH;

    struct Outcome {
    public:
        int m_delta_gold;
        int m_delta_health;
        std::vector<::runebound::map::TypeCell> m_necessary_result;

        Outcome() : m_delta_gold(0), m_delta_health(0) {
        }

        Outcome(
            int delta_gold,
            int delta_health,
            std::vector<runebound::map::TypeCell> necessary_result
        )
            : m_delta_gold(delta_gold),
              m_delta_health(delta_health),
              m_necessary_result(std::move(necessary_result)) {
        }
    };

    CardResearch(
        int task_position_x,
        int task_position_y,
        std::vector<Outcome> outcomes
    )
        : m_task_position_x(task_position_x),
          m_task_position_y(task_position_y),
          m_outcomes(std::move(outcomes)) {
    }

    CardResearch()
        : m_task_position_x(0), m_task_position_y(0), m_completed(false) {
    }

    CardResearch(const CardResearch &other)
        : m_task_position_x(0), m_task_position_y(0), m_completed(false) {
        *this = other;
    }

    CardResearch(CardResearch &&other) noexcept
        : m_task_position_x(0), m_task_position_y(0), m_completed(false) {
        *this = std::move(other);
    }

    CardResearch &operator=(const CardResearch &other) {
        m_task_position_x = other.m_task_position_x;
        m_task_position_y = other.m_task_position_y;
        m_completed = other.m_completed;
        m_outcomes = other.m_outcomes;
        return *this;
    }

    CardResearch &operator=(CardResearch &&other) noexcept {
        m_task_position_x = other.m_task_position_x;
        m_task_position_y = other.m_task_position_y;
        m_completed = other.m_completed;
        m_outcomes = std::move(other.m_outcomes);
        return *this;
    }

    [[nodiscard]] bool check_outcome(
        int index,
        std::vector<::runebound::dice::HandDice> &result_dice,
        int &delta_gold,
        int &delta_health
    );

    [[nodiscard]] int get_task_position_x() const {
        return m_task_position_x;
    }

    [[nodiscard]] int get_task_position_y() const {
        return m_task_position_y;
    }

    [[nodiscard]] std::vector<Outcome> get_outcomes() const {
        return m_outcomes;
    }

    nlohmann::json to_json() override;

    friend void to_json(nlohmann::json &json, const CardResearch &card);

    friend void from_json(const nlohmann::json &json, CardResearch &card);
};
}  // namespace cards
}  // namespace runebound
#endif  // CARD_RESEARCH_HPP_
