#ifndef CARD_FIGHT_HPP_
#define CARD_FIGHT_HPP_

#include "card_adventure.hpp"
#include "fight.hpp"
#include <string>
#include "runebound_fwd.hpp"
#include "nlohmann/json.hpp"

namespace runebound::cards {

struct CardFight;

void to_json(nlohmann::json &json, const CardFight &card);
void from_json(const nlohmann::json &json, CardFight &card);


struct CardFight : CardAdventure {
private:
    ::runebound::fight::Enemy m_enemy;
    int m_gold_award;
public:
     ::runebound::AdventureType m_card_type =
        ::runebound::AdventureType::FIGHT;

     CardFight() : m_gold_award(0) {}

     CardFight(int health, const std::string &name_enemy, int gold) : m_enemy(fight::Enemy(health, name_enemy)), m_gold_award(gold) {}

     [[nodiscard]] fight::Enemy get_enemy() const {
         return m_enemy;
     }

     [[nodiscard]]  int get_gold_award() const {
         return m_gold_award;
     }

     friend void to_json(nlohmann::json &json, const CardFight &card);
     friend void from_json(const nlohmann::json &json, CardFight &card);

};


} // namespace runebound::cards
#endif  // CARD_FIGHT_HPP_
