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
    std::vector<FightToken> m_fight_tokens;

public:
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
                    damage -= 1;
                    shields.push_back(m_character_remaining_tokens[i]);
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

    static bool check_combination_tokens(
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
        std::vector<FightToken> character_tokens = m_enemy.get_fight_token();
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

public:
    void make_progress(
        Participant participant,
        const std::vector<TokenHandCount> &tokens,
        std::optional<FightToken> dexterity_token,
        std::optional<Participant> dexterity_participant
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
                        Participant::ENEMY, static_cast<int>(tokens.size())
                    );
                    break;
                }
                case (HandFightTokens::MAGICAL_DAMAGE): {
                    make_damage(
                        Participant::ENEMY, static_cast<int>(tokens.size())
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
                default: {
                    m_turn =
                        static_cast<Participant>(static_cast<int>(m_turn) ^ 1);
                    throw BadCombinationException();
                }
            }
        } else {
            switch (tokens[0].hand) {
                case (HandFightTokens::ENEMY_DAMAGE): {
                    make_damage(
                        Participant::CHARACTER, static_cast<int>(tokens.size())
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

                default: {
                    m_turn =
                        static_cast<Participant>(static_cast<int>(m_turn) ^ 1);
                    throw BadCombinationException();
                }
            }
        }
    }

    HandFightTokens
    toss_token(Participant participant, const FightToken &token) {
        if (participant == Participant::ENEMY) {
            for (auto &enemy_token : m_enemy_remaining_tokens) {
                if (token == enemy_token.token) {
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
            if (token == character_token.token) {
                if (rng() % 2 == 0) {
                    character_token.hand = character_token.token.first;
                } else {
                    character_token.hand = character_token.token.second;
                }
                return character_token.hand;
            }
        }
    }

    HandFightTokens
    reverse_token(Participant participant, const FightToken &token) {
        if (participant == Participant::ENEMY) {
            for (auto &enemy_token : m_enemy_remaining_tokens) {
                if (token == enemy_token.token) {
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
            if (token == character_token.token) {
                if (character_token.hand == character_token.token.second) {
                    character_token.hand = character_token.token.first;
                } else {
                    character_token.hand = character_token.token.second;
                }
                return character_token.hand;
            }
        }
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
