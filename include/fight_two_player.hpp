#include <memory>
#include "character.hpp"
#include "fight.hpp"

namespace runebound::fight {

enum class ParticipantTwoPlayers { CALLER, RECEIVER };

struct FightTwoPlayer {
private:
    std::shared_ptr<::runebound::character::Character> m_receiver;
    Fight m_fight;

public:
    FightTwoPlayer(
        std::shared_ptr<character::Character> caller,
        std::shared_ptr<character::Character> receiver
    )
        : m_receiver(std::move(receiver)),
          m_fight(
              std::move(caller),
              Enemy(receiver->get_health(), receiver->get_fight_token())
          ) {
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
        Participant dexterity_participant
    ) {
        m_fight.make_dexterity(
            static_cast<Participant>(participant), token, dexterity_token,
            dexterity_participant
        );
        m_receiver->set_health(m_fight.get_health_enemy());
    }

    void start_round() {
        m_fight.start_round();
    }

    [[nodiscard]] bool check_end_fight() const {
        return m_fight.check_end_fight();
    }

    [[nodiscard]] ParticipantTwoPlayers get_winner() const {
        return static_cast<ParticipantTwoPlayers>(m_fight.get_winner());
    }

    bool check_end_round() {
        return m_fight.check_end_round();
    }
};
}  // namespace runebound::fight