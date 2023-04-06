#ifndef FIGHT_HPP_
#define FIGHT_HPP_

#include <memory>
#include <vector>
#include "character.hpp"
#include "runebound_fwd.hpp"
#include <exception>
#include "fight_token.hpp"

namespace runebound::fight {

struct BadCombinationException : std::runtime_error {
    BadCombinationException() : std::runtime_error("Wrong combination of tokens") {}
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

    [[nodiscard]] std::vector <FightToken> get_fight_token() const {
        return m_fight_tokens;
    }
};

struct Fight {
private:
    friend struct Character;
    friend struct Enemy;
    std::shared_ptr<::runebound::character::Character> m_character;
    ::runebound::fight::Enemy m_enemy;
    std::vector <HandFightTokens> m_enemy_hand_tokens;
    std::vector <HandFightTokens> m_character_hand_tokens;
    std::vector <std::pair <FightToken, HandFightTokens>> m_enemy_remaining_tokens;
    std::vector <std::pair <FightToken, HandFightTokens>> m_character_remaining_tokens;

    void update_state_character(int delta_health) {
        m_character->update_health(delta_health);
    }

    void update_state_enemy(int delta_health) {
        m_enemy.update_health(delta_health);
    }

    bool make_damage(Participant participant, int damage) {
        if (participant == Participant::CHARACTER) {
            std::vector <std::pair <FightToken, HandFightTokens>> shields;
            for (std::size_t i = 0; i < m_character_remaining_tokens.size(); ++i) {
                if (m_character_remaining_tokens[i].second == HandFightTokens::SHIELD) {
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
        std::vector <std::pair <FightToken, HandFightTokens>> shields;
        for (std::size_t i = 0; i < m_enemy_remaining_tokens.size(); ++i) {
            if (m_enemy_remaining_tokens[i].second == HandFightTokens::SHIELD) {
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

    void erase_token(Participant participant, std::pair <FightToken, HandFightTokens> token) {
        if (participant == Participant::ENEMY) {
            m_enemy_remaining_tokens.erase(std::find(m_enemy_remaining_tokens.begin(), m_enemy_remaining_tokens.end(), token));
        }
        else {
            m_character_remaining_tokens.erase(std::find(m_character_remaining_tokens.begin(), m_character_remaining_tokens.end(), token));
        }
    }

    void erase_token(Participant participant, std::pair <FightToken, HandFightTokens> token, unsigned int count_tokens) {
        for (unsigned int i = 0; i < count_tokens; ++i) {
            erase_token(participant, token);
        }
    }

    bool check_combination_tokens(const std::vector <std::pair<FightToken, HandFightTokens>> &tokens) {
        for (std::size_t i = 1; i < tokens.size(); ++i) {
            if (tokens[i].second != tokens[i - 1].second) {
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
                m_character_remaining_tokens.push_back({character_token, character_token.first});
            }
            else {
                m_character_remaining_tokens.push_back({character_token, character_token.second});
            }
        }
        for (auto &enemy_token : enemy_tokens) {
            if (rng() % 2 == 0) {
                m_enemy_remaining_tokens.push_back({enemy_token, enemy_token.first});
            }
            else {
                m_enemy_remaining_tokens.push_back({enemy_token, enemy_token.second});
            }
        }
    }
public:
    void make_progress(Participant participant, const std::vector <std::pair <FightToken, HandFightTokens>> &tokens) {
        if (!check_combination_tokens(tokens)) {
            throw BadCombinationException();
        }
        if (participant == Participant::CHARACTER) {
            switch (tokens[0].second) {
                case (HandFightTokens::PHYSICAL_DAMAGE): {
                    make_damage(Participant::ENEMY, static_cast<int>(tokens.size()));
                    break;
                }
                case (HandFightTokens::MAGICAL_DAMAGE): {
                    make_damage(Participant::ENEMY, static_cast<int>(tokens.size()));
                    break;
                }

                default: {
                    throw BadCombinationException();
                }
            }
        }
        else {
            switch (tokens[0].second) {
                case (HandFightTokens::ENEMY_DAMAGE): {
                    make_damage(Participant::CHARACTER, static_cast<int>(tokens.size()));
                    break;
                }
                default: {
                    throw BadCombinationException();
                }
            }
        }
    }

    HandFightTokens toss_token(Participant participant, const FightToken &token) {
        if (participant == Participant::ENEMY) {
            for (auto &enemy_token : m_enemy_remaining_tokens) {
                if (token == enemy_token.first) {
                    if (rng() % 2 == 0) {
                        enemy_token.second = enemy_token.first.first;
                    }
                    else {
                        enemy_token.second = enemy_token.first.second;
                    }
                    return enemy_token.second;
                }
            }
        }
        for (auto &character_token : m_character_remaining_tokens) {
            if (token == character_token.first) {
                if (rng() % 2 == 0) {
                    character_token.second = character_token.first.first;
                }
                else {
                    character_token.second = character_token.first.second;
                }
                return character_token.second;
            }
        }
    }

    HandFightTokens reverse_token(Participant participant, const FightToken &token) {
        if (participant == Participant::ENEMY) {
            for (auto &enemy_token : m_enemy_remaining_tokens) {
                if (token == enemy_token.first) {
                    if (enemy_token.second == enemy_token.first.second) {
                        enemy_token.second = enemy_token.first.first;
                    }
                    else {
                        enemy_token.second = enemy_token.first.second;
                    }
                    return enemy_token.second;
                }
            }
        }
        for (auto &character_token : m_character_remaining_tokens) {
            if (token == character_token.first) {
                if (character_token.second == character_token.first.second) {
                    character_token.second = character_token.first.first;
                }
                else {
                    character_token.second = character_token.first.second;
                }
                return character_token.second;
            }
        }
    }

    void start_round() {
        shuffle_all_tokens();
    }

};
}  // namespace runebound::fight

#endif  // FIGHT_HPP_
