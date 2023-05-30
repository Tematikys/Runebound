#include "fight_two_player.hpp"
#include "runebound_fwd.hpp"
#include "game.hpp"
#include "nlohmann/json.hpp"

namespace runebound::fight {

void to_json(nlohmann::json &json, const FightTwoPlayer &fight) {
    json["m_receiver"] = fight.m_receiver->get_standard_character();
    json["m_caller"] = fight.m_caller->get_standard_character();
    json["m_fight"] = fight.m_fight;
}

void from_json(const nlohmann::json &json, FightTwoPlayer &fight, const game::Game &game) {
    fight.m_receiver = game.get_character(json["m_receiver"]);
    fight.m_receiver = game.get_character(json["m_receiver"]);
    fight::from_json(json["m_fight"], fight.m_fight, game);
}


} // namespace runebound::fight