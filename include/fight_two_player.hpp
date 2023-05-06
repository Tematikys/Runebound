#include "fight.hpp"
#include <memory>
#include "character.hpp"

namespace runebound::fight {

enum class ParticipantTwoPlayers { CALLER, RECEIVER };

struct FightTwoPlayer : Fight {
private:
    std::shared_ptr<::runebound::character::Character> m_receiver;

public:
    FightTwoPlayer(std::shared_ptr<character::Character> caller, std::shared_ptr<character::Character> receiver) :
          m_receiver(std::move(receiver)), Fight(std::move(caller), Enemy(receiver->get_health(), receiver->get_fight_token())) {}

    void pass_caller() {
        pass_character();
    }

    void pass_receiver() {
        pass_enemy();
    }

    [[nodiscard]] std::vector<TokenHandCount> get_caller_remaining_tokens(
    ) const {
        return get_character_remaining_tokens();
    }

    [[nodiscard]] std::vector<TokenHandCount> get_receiver_remaining_tokens(
    ) const {
        return get_enemy_remaining_tokens();
    }

    [[nodiscard]] ParticipantTwoPlayers get_turn_character_characters() const {
        return static_cast <ParticipantTwoPlayers>(get_turn());
    }

    void make_doubling_characters(
        ParticipantTwoPlayers participant,
        const TokenHandCount &token,
        TokenHandCount doubling_token
    ) {
        make_doubling(static_cast <Participant>(Participant::CHARACTER), token, doubling_token);
    }

    void make_damage_characters(
        ParticipantTwoPlayers participant,
        const std::vector<TokenHandCount> &tokens
    );




};
}