#ifndef CARD_RESEARCH_HPP_
#define CARD_RESEARCH_HPP_
#include <algorithm>
#include <map>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <vector>
#include "card_adventure.hpp"
#include "dice.hpp"
#include "map_cell.hpp"
#include "point.hpp"

namespace runebound {
namespace cards {
void to_json(nlohmann::json &json, const CardResearch &card);
void from_json(const nlohmann::json &json, CardResearch &card, map::Map &map);

struct CardResearch : CardAdventure {
public:
    struct Outcome;

private:
    std::string m_required_territory;
    std::set<Point> m_required_cells;
    bool m_completed = false;
    std::string m_card_name;
    std::vector<Outcome> m_outcomes;

    [[nodiscard]] bool check_completion_task(const Point &cell);

public:
    ::runebound::AdventureType m_card_type =
        ::runebound::AdventureType::RESEARCH;

    struct Outcome {
    public:
        int m_delta_gold;
        int m_delta_health;
        int m_knowledge_token;
        std::vector<::runebound::map::TypeCell> m_necessary_result;

        Outcome() : m_delta_gold(0), m_delta_health(0) {
        }

        Outcome(
            int delta_gold,
            int delta_health,
            int knowledge_token,
            std::vector<runebound::map::TypeCell> necessary_result
        )
            : m_delta_gold(delta_gold),
              m_delta_health(delta_health),
              m_knowledge_token(knowledge_token),
              m_necessary_result(std::move(necessary_result)) {
        }
    };

    CardResearch(
        std::string card_name,
        std::string territory,
        std::vector<Outcome> outcomes
    )
        : m_card_name(std::move(card_name)),
          m_required_territory(std::move(territory)),
          m_outcomes(std::move(outcomes)) {
    }

    CardResearch() : m_completed(false) {
    }

    [[nodiscard]] int get_delta_gold(int index) const {
        return m_outcomes[index].m_delta_gold;
    }

    [[nodiscard]] int get_delta_health(int index) const {
        return m_outcomes[index].m_delta_health;
    }

    [[nodiscard]] int get_knowledge_token(int index) const {
        return m_outcomes[index].m_knowledge_token;
    }

    [[nodiscard]] bool check_outcome(
        int index,
        std::vector<::runebound::dice::HandDice> &result_dice
    ) const;

    [[nodiscard]] std::string get_required_territory() const {
        return m_required_territory;
    }

    [[nodiscard]] std::vector<Outcome> get_outcomes() const {
        return m_outcomes;
    }

    friend void to_json(nlohmann::json &json, const CardResearch &card);

    friend void
    from_json(const nlohmann::json &json, CardResearch &card, map::Map &map);
};
}  // namespace cards
}  // namespace runebound
#endif  // CARD_RESEARCH_HPP_
