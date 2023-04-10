#ifndef FIGHT_HPP_
#define FIGHT_HPP_

#include <exception>
#include <iostream>
#include <memory>
#include <vector>
#include "character.hpp"
#include "fight_token.hpp"
#include "runebound_fwd.hpp"

namespace runebound::fight {

struct BadCombinationException : std::runtime_error {
    BadCombinationException()
        : std::runtime_error("Wrong combination of tokens") {
    }
};

struct WrongCharacterTurnException : std::runtime_error {
    WrongCharacterTurnException() : std::runtime_error("Wrong character turn") {
    }
};

struct TokenHandCount {
public:
    FightToken token;
    HandFightTokens hand;
    int count;

    TokenHandCount(FightToken token_, HandFightTokens hand_, int count_)
        : token(token_), hand(hand_), count(count_) {
    }

    bool operator==(const TokenHandCount &token_hand_count) const {
        return token == token_hand_count.token &&
               hand == token_hand_count.hand && count == token_hand_count.count;
    }
};

enum class Participant { CHARACTER, ENEMY };

struct Enemy {
private:
    int m_health;
    const std::string m_name;
    std::vector<FightToken> m_fight_tokens = {
        FightToken(
            HandFightTokens::ENEMY_DAMAGE,
            0,
            HandFightTokens::DEXTERITY,
            0
        ),
        FightToken(HandFightTokens::NOTHING, 1, HandFightTokens::DOUBLING, 0),
        FightToken(
            HandFightTokens::SHIELD,
            0,
            HandFightTokens::ENEMY_DAMAGE,
            0
        ),
        FightToken(
            HandFightTokens::ENEMY_DAMAGE,
            0,
            HandFightTokens::NOTHING,
            1
        ),
        FightToken(
            HandFightTokens::ENEMY_DAMAGE,
            0,
            HandFightTokens::ENEMY_DAMAGE,
            1
        ),  // THERE HIT
    };

public:
    Enemy(int health, std::string name)
        : m_health(health), m_name(std::move(name)) {
    }

    void update_health(int delta) {
        m_health += delta;
    }

    int get_health() const {
        return m_health;
    }

    [[nodiscard]] std::vector<FightToken> get_fight_token() const {
        return m_fight_tokens;
    }
};

struct Fight {
private:
    friend struct Character;
    friend struct Enemy;
    Participant m_turn;
    std::shared_ptr<::runebound::character::Character> m_character;
    ::runebound::fight::Enemy m_enemy;
    std::vector<TokenHandCount> m_enemy_remaining_tokens;
    std::vector<TokenHandCount> m_character_remaining_tokens;

    void update_state_character(int delta_health) {
        m_character->update_health(delta_health);
    }

    void update_state_enemy(int delta_health) {
        m_enemy.update_health(delta_health);
    }

    bool make_damage(Participant participant, int damage);

    void erase_token(Participant participant, TokenHandCount token);

    bool check_combination_tokens(const std::vector<TokenHandCount> &tokens);

    void shuffle_all_tokens();

    void
    make_doubling(Participant participant, const TokenHandCount &fight_token);

    static unsigned int count_initiative(
        const std::vector<TokenHandCount> &tokens
    );

    static int count_damage(const std::vector<TokenHandCount> &tokens);

public:
    Fight(std::shared_ptr<character::Character> character, Enemy enemy)
        : m_character(std::move(character)), m_enemy(std::move(enemy)) {
    }

    bool check_end_round() const;

    [[nodiscard]] int get_health_enemy() const {
        return m_enemy.get_health();
    }

    void make_progress(
        Participant participant,
        const std::vector<TokenHandCount> &tokens,
        std::optional<TokenHandCount> dexterity_token,
        std::optional<Participant> dexterity_participant,
        std::optional<TokenHandCount> doubling_token
    );

    bool check_end_fight() const {
        return m_character->get_health() == 0 || m_enemy.get_health() == 0;
    }

    HandFightTokens
    toss_token(Participant participant, const TokenHandCount &token);

    HandFightTokens
    reverse_token(Participant participant, const TokenHandCount &token);

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

    void start_round();
};
}  // namespace runebound::fight

#endif  // FIGHT_HPP_
