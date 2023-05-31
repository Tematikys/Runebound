#ifndef FIGHT_TWO_PLAYER_HPP_
#define FIGHT_TWO_PLAYER_HPP_

#include <memory>
#include "character.hpp"
#include "fight.hpp"
#include "runebound_fwd.hpp"

namespace runebound::fight {

enum class ParticipantTwoPlayers { CALLER, RECEIVER };

void to_json(nlohmann::json &json, const FightTwoPlayer &fight);
void from_json(const nlohmann::json &json, FightTwoPlayer &fight, const game::Game &game);

struct FightTwoPlayer {
private:
    std::shared_ptr<::runebound::character::Character> m_receiver;
    std::shared_ptr<::runebound::character::Character> m_caller;
    Fight m_fight;

public:
    FightTwoPlayer() = default;

    FightTwoPlayer(
        const std::shared_ptr<character::Character> &caller,
        const std::shared_ptr<character::Character> &receiver
    )
        : m_receiver(receiver),
          m_caller(caller),
          m_fight(
              caller,
              Enemy(receiver->get_health(), receiver->get_fight_token())
          ) {
    }

    [[nodiscard]] std::shared_ptr<character::Character> get_caller() const {
        return m_caller;
    }

    [[nodiscard]] std::shared_ptr<character::Character> get_receiver() const {
        return m_receiver;
    }

    void pass_caller() {
        m_fight.pass_character();
    }

    void pass_receiver() {
        m_fight.pass_enemy();
    }

    [[nodiscard]] std::vector<TokenHandCount> get_caller_remaining_tokens(
    ) const {
        return m_fight.get_character_remaining_tokens();
    }

    [[nodiscard]] std::vector<TokenHandCount> get_receiver_remaining_tokens(
    ) const {
        return m_fight.get_enemy_remaining_tokens();
    }

    [[nodiscard]] ParticipantTwoPlayers get_turn() const {
        return static_cast<ParticipantTwoPlayers>(m_fight.get_turn());
    }

    void make_doubling(
        ParticipantTwoPlayers participant,
        const TokenHandCount &token,
        TokenHandCount doubling_token
    ) {
        m_fight.make_doubling(
            static_cast<Participant>(participant), token, doubling_token
        );
        m_receiver->set_health(m_fight.get_health_enemy());
    }

    void make_damage(
        ParticipantTwoPlayers participant,
        const std::vector<TokenHandCount> &tokens
    ) {
        m_fight.make_damage(static_cast<Participant>(participant), tokens);
        m_receiver->set_health(m_fight.get_health_enemy());
    }

    void make_dexterity(
        ParticipantTwoPlayers participant,
        const TokenHandCount &token,
        TokenHandCount dexterity_token,
        ParticipantTwoPlayers dexterity_participant
    ) {
        m_fight.make_dexterity(
            static_cast<Participant>(participant), token, dexterity_token,
            static_cast<Participant>(dexterity_participant)
        );
        m_receiver->set_health(m_fight.get_health_enemy());
    }

    void start_round() {
        m_fight.start_round();
    }

    [[nodiscard]] bool check_end_fight() const {
        return m_caller->get_health() == 0 || m_receiver->get_health() == 0;
    }

    [[nodiscard]] ParticipantTwoPlayers get_winner() const {
        if (m_caller->get_health() == 0) {
            return ParticipantTwoPlayers::RECEIVER;
        }
        return ParticipantTwoPlayers::CALLER;
    }

    bool check_end_round() {
        return m_fight.check_end_round();
    }

    friend void to_json(nlohmann::json &json, const FightTwoPlayer &fight);

    friend void from_json(const nlohmann::json &json, FightTwoPlayer &fight, const game::Game &game);
};
}  // namespace runebound::fight

#endif  // FIGHT_TWO_PLAYER_HPP_
