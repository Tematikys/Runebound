
#ifndef RUNEBOUND_FIGHT_CLIENT_HPP
#define RUNEBOUND_FIGHT_CLIENT_HPP
#include "fight.hpp"

namespace runebound::fight {
struct FightClient {
    friend struct Character;

    Participant m_turn = Participant::CHARACTER;
//    bool is_round_end = false;
    bool m_pass_character = false;
    bool m_pass_enemy = false;
    ::runebound::character::Character m_character;
    Enemy m_enemy;
    std::vector<TokenHandCount> m_enemy_remaining_tokens;
    std::vector<TokenHandCount> m_character_remaining_tokens;
    FightClient() = default;

    FightClient(Fight &fight) {
        m_turn = fight.m_turn;
        m_pass_character = fight.m_pass_character;
        m_pass_enemy = fight.m_pass_enemy;
        m_character = *(fight.m_character);
        m_enemy = *(fight.get_enemy());
        m_enemy_remaining_tokens = fight.m_enemy_remaining_tokens;
        m_character_remaining_tokens = fight.m_character_remaining_tokens;

//        is_round_end = fight.check_end_round();
    }

    bool check_end_fight() const {
        return m_character.get_health() == 0 || m_enemy.get_health() == 0;
    }

//    bool check_end_round() const {
//        return is_round_end;
//    }

    [[nodiscard]] Participant get_turn() const {
        return m_turn;
    }

    [[nodiscard]] std::vector<TokenHandCount> get_character_remaining_tokens(
    ) const {
        return m_character_remaining_tokens;
    }

    [[nodiscard]] std::vector<TokenHandCount> get_enemy_remaining_tokens(
    ) const {
        return m_enemy_remaining_tokens;
    }

    friend void to_json(nlohmann::json &json, const FightClient &fight);
    friend void from_json(const nlohmann::json &json, FightClient &fight);
};

}  // namespace runebound::fight

#endif  // RUNEBOUND_FIGHT_CLIENT_HPP
