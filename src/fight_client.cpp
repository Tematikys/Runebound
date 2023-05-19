#include "fight_client.hpp"

namespace runebound::fight {
void to_json(nlohmann::json &json, const FightClient &fight) {
    json["m_turn"] = fight.m_turn;
    json["m_pass_character"] = fight.m_pass_character;
    json["m_pass_enemy"] = fight.m_pass_enemy;
    json["m_character"] = fight.m_character;
    json["m_enemy"] = fight.m_enemy;

    json["m_enemy_remaining_tokens"] = fight.m_enemy_remaining_tokens;
    json["m_character_remaining_tokens"] = fight.m_character_remaining_tokens;

//    json["is_round_end"] = fight.is_round_end;
}

void from_json(const nlohmann::json &json, FightClient &fight) {
    fight.m_turn = json["m_turn"];
    fight.m_pass_character = json["m_pass_character"];
    fight.m_pass_enemy = json["m_pass_enemy"];
    fight.m_character = json["m_character"];
    fight.m_enemy = json["m_enemy"];

    fight.m_enemy_remaining_tokens = json["m_enemy_remaining_tokens"];
    fight.m_character_remaining_tokens = json["m_character_remaining_tokens"];

//    fight.is_round_end = json["is_round_end"];
}
}  // namespace runebound::fight