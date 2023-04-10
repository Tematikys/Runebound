#include "fight.hpp"
#include <vector>


namespace runebound::fight {

bool Fight::make_damage(Participant participant, int damage) {
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

void Fight::erase_token(Participant participant, TokenHandCount token) {
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

void Fight::make_doubling(Participant participant, const TokenHandCount &fight_token) {
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

unsigned int Fight::count_initiative(
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

int Fight::count_damage(const std::vector <TokenHandCount> &tokens) {
    int count = 0;
    for (const auto &token : tokens) {
        count += token.count;
    }
    return count;
}
} // namespace runebound::fights