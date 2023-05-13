#ifndef FIGHT_HPP_
#define FIGHT_HPP_

#include <exception>
#include <functional>
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

struct FightIsStillOnException : std::runtime_error {
    FightIsStillOnException() : std::runtime_error("The fight is still on") {
    }
};

void to_json(nlohmann::json &json, const TokenHandCount &token_hand_count);
void from_json(const nlohmann::json &json, TokenHandCount &token_hand_count);

struct TokenHandCount {
public:
    FightToken token;
    HandFightTokens hand = HandFightTokens::NOTHING;
    int count = 0;

    TokenHandCount() = default;

    TokenHandCount(FightToken token_, HandFightTokens hand_, int count_)
        : token(token_), hand(hand_), count(count_) {
    }

    bool operator==(const TokenHandCount &token_hand_count) const {
        return token == token_hand_count.token &&
               hand == token_hand_count.hand && count == token_hand_count.count;
    }

    friend void
    to_json(nlohmann::json &json, const TokenHandCount &token_hand_count) {
        json["token"] = token_hand_count.token;
        json["hand"] = token_hand_count.hand;
        json["count"] = token_hand_count.count;
    }

    friend void
    from_json(const nlohmann::json &json, TokenHandCount &token_hand_count) {
        token_hand_count.count = json["count"];
        token_hand_count.hand = json["hand"];
        token_hand_count.token = json["token"];
    }
};

enum class Participant { CHARACTER, ENEMY };

void to_json(nlohmann::json &json, const Enemy &enemy);
void from_json(const nlohmann::json &json, Enemy &enemy);

struct Enemy {
private:
    bool m_boss = false;
    int m_health, m_max_health;
    std::function<void()> m_hit = []() {};
    std::string m_name;

    std::vector<FightToken> m_fight_tokens = {
        FightToken(
            HandFightTokens::ENEMY_DAMAGE,
            0,
            1,
            HandFightTokens::DEXTERITY,
            0,
            1
        ),
        FightToken(
            HandFightTokens::NOTHING,
            1,
            1,
            HandFightTokens::DOUBLING,
            0,
            1
        ),
        FightToken(
            HandFightTokens::SHIELD,
            0,
            1,
            HandFightTokens::ENEMY_DAMAGE,
            0,
            1
        ),
        FightToken(
            HandFightTokens::ENEMY_DAMAGE,
            0,
            1,
            HandFightTokens::NOTHING,
            1,
            1
        ),
        FightToken(
            HandFightTokens::ENEMY_DAMAGE,
            0,
            1,
            HandFightTokens::ENEMY_DAMAGE,
            1,
            1
        )  // THERE HIT
    };

public:
    Enemy() : m_health(0), m_max_health(0) {
    }

    Enemy(int health, std::vector<FightToken> fight_tokens)
        : m_health(health),
          m_fight_tokens(std::move(fight_tokens)),
          m_max_health(health) {
    }

    Enemy(int health, std::string name)
        : m_health(health), m_name(std::move(name)) {
    }

    explicit Enemy(AdventureType boss) {
        if (boss == AdventureType::BOSS) {
            m_health = 15;
            m_max_health = 15;
            m_name = "Supreme Lord Marat";
            m_fight_tokens.emplace_back(FightToken(
                HandFightTokens::DEXTERITY, 1, 1, HandFightTokens::ENEMY_DAMAGE,
                0, 2
            ));
            m_fight_tokens.emplace_back(FightToken(
                HandFightTokens::DEXTERITY, 0, 1, HandFightTokens::ENEMY_DAMAGE,
                0, 2
            ));
            m_fight_tokens.emplace_back(FightToken(
                HandFightTokens::HIT, 0, 1, HandFightTokens::ENEMY_DAMAGE, 0, 1
            ));
            m_hit = std::function<void()>([this] {
                m_health = std::min(m_health + 3, m_max_health);
            });
            m_boss = true;
        } else {
            m_health = 0;
            m_max_health = 0;
        }
    }

    void update_health(int delta) {
        m_health += delta;
    }

    void make_hit() {
        m_hit();
    }

    [[nodiscard]] bool check_boss() const {
        return m_boss;
    }

    [[nodiscard]] int get_health() const {
        return m_health;
    }

    [[nodiscard]] std::string get_name() const {
        return m_name;
    }

    [[nodiscard]] std::vector<FightToken> get_fight_token() const {
        return m_fight_tokens;
    }

    friend void to_json(nlohmann::json &json, const Enemy &enemy) {
        json["m_name"] = enemy.m_name;
        json["m_health"] = enemy.m_health;
    }

    friend void from_json(const nlohmann::json &json, Enemy &enemy) {
        enemy.m_name = json["m_name"];
        enemy.m_health = json["m_health"];
    }
};

struct Fight {
private:
    friend struct FightClient;
    friend struct Character;
    friend struct Enemy;
    bool m_fight_started = false;
    Participant m_turn = Participant::CHARACTER;
    bool m_pass_character = false;
    bool m_pass_enemy = false;
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

    void make_doubling_private(
        Participant participant,
        const TokenHandCount &fight_token
    );

    static unsigned int count_initiative(
        const std::vector<TokenHandCount> &tokens
    );

    static int count_damage(const std::vector<TokenHandCount> &tokens);

    void change_turn();

    [[nodiscard]] bool check_end_round_private(
        const std::vector<TokenHandCount> &tokens
    ) const;

    HandFightTokens
    toss_token(Participant participant, const TokenHandCount &token);

    HandFightTokens
    reverse_token(Participant participant, const TokenHandCount &token);

public:
    Fight(std::shared_ptr<character::Character> character, Enemy enemy)
        : m_character(std::move(character)), m_enemy(std::move(enemy)) {
    }

    [[nodiscard]] Participant get_winner() const {
        if (!check_end_fight()) {
            throw FightIsStillOnException();
        }
        return m_turn;
    }

    [[nodiscard]] Enemy *get_enemy() {
        return &m_enemy;
    }

    void pass_character() {
        m_pass_character = true;
    }

    void pass_enemy() {
        m_pass_enemy = true;
    }

    bool check_end_round();

    [[nodiscard]] int get_health_enemy() const {
        return m_enemy.get_health();
    }

    void make_dexterity(
        Participant participant,
        const TokenHandCount &token,
        TokenHandCount dexterity_token,
        Participant dexterity_participant
    );

    void make_doubling(
        Participant participant,
        const TokenHandCount &token,
        TokenHandCount doubling_token
    );

    void make_damage(
        Participant participant,
        const std::vector<TokenHandCount> &tokens
    );

    void make_hit(Participant participant, const TokenHandCount &token);

    [[nodiscard]] bool check_end_fight() const {
        return m_character->get_health() == 0 || m_enemy.get_health() == 0;
    }

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
