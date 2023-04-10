#ifndef FIGHT_HPP_
#define FIGHT_HPP_

#include <exception>
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
        FightToken(HandFightTokens::ENEMY_DAMAGE, 0, HandFightTokens::DEXTERITY, 0),
        FightToken(HandFightTokens::NOTHING, 1, HandFightTokens::DOUBLING, 0),
        FightToken(HandFightTokens::SHIELD, 0, HandFightTokens::ENEMY_DAMAGE, 0),
        FightToken(HandFightTokens::ENEMY_DAMAGE, 0, HandFightTokens::NOTHING, 1),
        FightToken(HandFightTokens::ENEMY_DAMAGE, 0, HandFightTokens::ENEMY_DAMAGE, 1), // THERE HIT
    };

public:
    Enemy(int health, std::string name) : m_health(health), m_name(std::move(name)) {}

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

    bool make_damage(Participant participant, int damage) {
        if (participant == Participant::CHARACTER) {
            std::vector<TokenHandCount> shields;
            for (std::size_t i = 0; i < m_character_remaining_tokens.size();
                 ++i) {
                if (m_character_remaining_tokens[i].hand ==
                    HandFightTokens::SHIELD) {
                    auto harm = std::min(m_character_remaining_tokens[i].count, damage);
                    damage -= harm;
                    m_character_remaining_tokens[i].count -= harm;
                    if (m_character_remaining_tokens[i].count == 0) {
                        shields.push_back(m_character_remaining_tokens[i]);
                    }
                    if (damage == 0) {
                        break;
                    }
                }
            }
            for (const auto &token : shields) {
                erase_token(Participant::CHARACTER, token);
            }
            if (m_character->get_health() <= damage) {
                m_character->update_health(-m_character->get_health());
                return true;
            }
            std::cout << damage << '\n';
            m_character->update_health(-damage);
            return false;
        }
        std::vector<TokenHandCount> shields;
        for (std::size_t i = 0; i < m_enemy_remaining_tokens.size(); ++i) {
            if (m_enemy_remaining_tokens[i].hand == HandFightTokens::SHIELD) {
                damage -= 1;
                shields.push_back(m_enemy_remaining_tokens[i]);
                if (damage == 0) {
                    break;
                }
            }
        }
        for (const auto &token : shields) {
            erase_token(Participant::ENEMY, token);
        }
        std::cout << damage << '\n';
        if (m_enemy.get_health() <= damage) {
            m_enemy.update_health(-m_enemy.get_health());
            return true;
        }
        m_enemy.update_health(-damage);
        return false;
    }

    void erase_token(Participant participant, TokenHandCount token) {
        if (participant == Participant::ENEMY) {
            m_enemy_remaining_tokens.erase(std::find(
                m_enemy_remaining_tokens.begin(),
                m_enemy_remaining_tokens.end(), token
            ));
        } else {
            m_character_remaining_tokens.erase(std::find(
                m_character_remaining_tokens.begin(),
                m_character_remaining_tokens.end(), token
            ));
        }
    }

    void erase_token(
        Participant participant,
        TokenHandCount token,
        unsigned int count_tokens
    ) {
        for (unsigned int i = 0; i < count_tokens; ++i) {
            erase_token(participant, token);
        }
    }

    bool check_combination_tokens(
        const std::vector<TokenHandCount> &tokens
    ) {
        for (std::size_t i = 1; i < tokens.size(); ++i) {
            if (tokens[i].hand != tokens[i - 1].hand) {
                return false;
            }
        }
        return true;
    }

    void shuffle_all_tokens() {
        m_character_remaining_tokens.clear();
        m_enemy_remaining_tokens.clear();
        std::vector<FightToken> enemy_tokens = m_enemy.get_fight_token();
        std::vector<FightToken> character_tokens = m_character.get()->get_fight_token();
        for (auto &character_token : character_tokens) {
            if (rng() % 2 == 0) {
                m_character_remaining_tokens.push_back(
                    {character_token, character_token.first, 1}
                );
            } else {
                m_character_remaining_tokens.push_back(
                    {character_token, character_token.second, 1}
                );
            }
        }
        for (auto &enemy_token : enemy_tokens) {
            if (rng() % 2 == 0) {
                m_enemy_remaining_tokens.push_back(
                    {enemy_token, enemy_token.first, 1}
                );
            } else {
                m_enemy_remaining_tokens.push_back(
                    {enemy_token, enemy_token.second, 1}
                );
            }
        }
    }

    void make_doubling(Participant participant, const TokenHandCount &fight_token) {
        if (participant == Participant::CHARACTER) {
            for (auto &token : m_character_remaining_tokens) {
                if (token == fight_token) {
                    token.count *= 2;
                    return;
                }
            }
        }
        else {
            for (auto &token : m_enemy_remaining_tokens) {
                if (token == fight_token) {
                    token.count *= 2;
                    return;
                }
            }
        }
    }

    static unsigned int count_initiative(
        const std::vector<TokenHandCount> &tokens
    ) {
        unsigned int count = 0;
        for (const auto &token : tokens) {
            if (token.token.first == token.hand) {
                if (token.token.first_lead) {
                    count += 1;
                }
            } else {
                if (token.token.second_lead) {
                    count += 1;
                }
            }
        }
        return count;
    }

    int count_damage(const std::vector <TokenHandCount> &tokens) const {
        int count = 0;
        for (const auto &token : tokens) {
            count += token.count;
        }
        return count;
    }

public:
    Fight(std::shared_ptr<character::Character> character, Enemy enemy) : m_character(std::move(character)), m_enemy(std::move(enemy)) {
    }

    bool check_end_round() const {
        bool only_bad_tokens = true;
        if (m_turn == Participant::CHARACTER) {
            for (const auto &token : m_character_remaining_tokens) {
                if (token.hand != HandFightTokens::NOTHING && token.hand != HandFightTokens::DOUBLING) {
                    only_bad_tokens = false;
                }
            }
            if (only_bad_tokens) {
                return true;
            }
            if (m_character_remaining_tokens.size() == 1 && m_character_remaining_tokens[0].hand == HandFightTokens::DEXTERITY) {
                return true;
            }
        }
        else {
            for (const auto &token : m_enemy_remaining_tokens) {
                if (token.hand != HandFightTokens::NOTHING && token.hand != HandFightTokens::DOUBLING) {
                    only_bad_tokens = false;
                }
            }
            if (only_bad_tokens) {
                return true;
            }
            if (m_enemy_remaining_tokens.size() == 1 && m_enemy_remaining_tokens[0].hand == HandFightTokens::DEXTERITY) {
                return true;
            }
        }
        return false;
    }

    [[nodiscard]] int get_health_enemy() const {
        return m_enemy.get_health();
    }
    void make_progress(
        Participant participant,
        const std::vector<TokenHandCount> &tokens,
        std::optional<TokenHandCount> dexterity_token,
        std::optional<Participant> dexterity_participant,
        std::optional<TokenHandCount> doubling_token
    ) {
        if (m_turn != participant) {
            throw WrongCharacterTurnException();
        }
        m_turn = static_cast<Participant>(static_cast<int>(m_turn) ^ 1);
        if (!check_combination_tokens(tokens)) {
            throw BadCombinationException();
        }
        if (participant == Participant::CHARACTER) {
            switch (tokens[0].hand) {
                case (HandFightTokens::PHYSICAL_DAMAGE): {
                    make_damage(
                        Participant::ENEMY, count_damage(tokens)
                    );
                    break;
                }
                case (HandFightTokens::MAGICAL_DAMAGE): {
                    make_damage(
                        Participant::ENEMY, count_damage(tokens)
                    );
                    break;
                }
                case (HandFightTokens::DEXTERITY): {
                    if (dexterity_participant == Participant::CHARACTER) {
                        reverse_token(
                            Participant::CHARACTER, dexterity_token.value()
                        );
                    } else {
                        toss_token(Participant::ENEMY, dexterity_token.value());
                    }
                    break;
                }
                case (HandFightTokens::DOUBLING): {
                    make_doubling(Participant::CHARACTER, doubling_token.value());
                    break;
                }
                case (HandFightTokens::NOTHING): {
                    break;
                }
                default: {
                    m_turn =
                        static_cast<Participant>(static_cast<int>(m_turn) ^ 1);
                    throw BadCombinationException();
                }
            }
            for (auto token : tokens) {
                erase_token(Participant::CHARACTER, token);
            }
        } else {
            switch (tokens[0].hand) {
                case (HandFightTokens::ENEMY_DAMAGE): {
                    make_damage(
                        Participant::CHARACTER, count_damage(tokens)
                    );
                    break;
                }
                case (HandFightTokens::DEXTERITY): {
                    if (dexterity_participant == Participant::CHARACTER) {
                        toss_token(
                            Participant::CHARACTER, dexterity_token.value()
                        );
                    } else {
                        reverse_token(
                            Participant::ENEMY, dexterity_token.value()
                        );
                    }
                    break;
                }
                case (HandFightTokens::DOUBLING): {
                    make_doubling(Participant::ENEMY, doubling_token.value());
                    break;
                }
                case (HandFightTokens::NOTHING): {
                    break;
                }
                default: {
                    m_turn =
                        static_cast<Participant>(static_cast<int>(m_turn) ^ 1);
                    throw BadCombinationException();
                }
            }
            for (auto token : tokens) {
                erase_token(Participant::ENEMY, token);
            }
        }
    }

    bool check_end_fight() const {
        return m_character->get_health() == 0 || m_enemy.get_health() == 0;
    }


    HandFightTokens
    toss_token(Participant participant, const TokenHandCount &token) {
        if (participant == Participant::ENEMY) {
            for (auto &enemy_token : m_enemy_remaining_tokens) {
                if (token == enemy_token) {
                    if (rng() % 2 == 0) {
                        enemy_token.hand = enemy_token.token.first;
                    } else {
                        enemy_token.hand = enemy_token.token.second;
                    }
                    return enemy_token.hand;
                }
            }
        }
        for (auto &character_token : m_character_remaining_tokens) {
            if (token == character_token) {
                if (rng() % 2 == 0) {
                    character_token.hand = character_token.token.first;
                } else {
                    character_token.hand = character_token.token.second;
                }
                return character_token.hand;
            }
        }
        return HandFightTokens::NOTHING;
    }


    HandFightTokens reverse_token(Participant participant, const TokenHandCount &token) {
        if (participant == Participant::ENEMY) {
            for (auto &enemy_token : m_enemy_remaining_tokens) {
                if (token == enemy_token) {
                    if (enemy_token.hand == enemy_token.token.second) {
                        enemy_token.hand = enemy_token.token.first;
                    } else {
                        enemy_token.hand = enemy_token.token.second;
                    }
                    return enemy_token.hand;
                }
            }
        }
        for (auto &character_token : m_character_remaining_tokens) {
            if (token == character_token) {
                if (character_token.hand == character_token.token.second) {
                    character_token.hand = character_token.token.first;
                } else {
                    character_token.hand = character_token.token.second;
                }
                return character_token.hand;
            }
        }
        return HandFightTokens::NOTHING;
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

    void start_round() {
        shuffle_all_tokens();
        unsigned int count_initiatives_character =
            count_initiative(m_character_remaining_tokens);
        unsigned int count_initiatives_enemy =
            count_initiative(m_enemy_remaining_tokens);
        if (count_initiatives_character >= count_initiatives_enemy) {
            m_turn = Participant::CHARACTER;
        } else {
            m_turn = Participant::ENEMY;
        }
    }
};
}  // namespace runebound::fight

#endif  // FIGHT_HPP_
