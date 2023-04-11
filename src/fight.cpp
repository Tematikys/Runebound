#include "fight.hpp"
#include <vector>

namespace runebound::fight {

bool Fight::make_damage(Participant participant, int damage) {
    if (participant == Participant::CHARACTER) {
        std::vector<TokenHandCount> shields;
        for (std::size_t i = 0; i < m_character_remaining_tokens.size(); ++i) {
            if (m_character_remaining_tokens[i].hand ==
                HandFightTokens::SHIELD) {
                auto harm =
                    std::min(m_character_remaining_tokens[i].count, damage);
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

void Fight::erase_token(Participant participant, TokenHandCount token) {
    if (participant == Participant::ENEMY) {
        m_enemy_remaining_tokens.erase(std::find(
            m_enemy_remaining_tokens.begin(), m_enemy_remaining_tokens.end(),
            token
        ));
    } else {
        m_character_remaining_tokens.erase(std::find(
            m_character_remaining_tokens.begin(),
            m_character_remaining_tokens.end(), token
        ));
    }
}

bool Fight::check_combination_tokens(const std::vector<TokenHandCount> &tokens
) {
    for (std::size_t i = 1; i < tokens.size(); ++i) {
        if (tokens[i].hand != tokens[i - 1].hand) {
            return false;
        }
    }
    return true;
}

void Fight::shuffle_all_tokens() {
    m_character_remaining_tokens.clear();
    m_enemy_remaining_tokens.clear();
    std::vector<FightToken> enemy_tokens = m_enemy.get_fight_token();
    std::vector<FightToken> character_tokens =
        m_character.get()->get_fight_token();
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

void Fight::make_doubling(
    Participant participant,
    const TokenHandCount &fight_token
) {
    if (participant == Participant::CHARACTER) {
        for (auto &token : m_character_remaining_tokens) {
            if (token == fight_token) {
                token.count *= 2;
                return;
            }
        }
    } else {
        for (auto &token : m_enemy_remaining_tokens) {
            if (token == fight_token) {
                token.count *= 2;
                return;
            }
        }
    }
}

unsigned int Fight::count_initiative(const std::vector<TokenHandCount> &tokens
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

int Fight::count_damage(const std::vector<TokenHandCount> &tokens) {
    int count = 0;
    for (const auto &token : tokens) {
        count += token.count;
    }
    return count;
}

bool Fight::check_end_round_private(const std::vector <TokenHandCount> &tokens) const {
    bool only_doubling = true;
    for (const auto &token : tokens) {
        if (token.hand != HandFightTokens::NOTHING &&
            token.hand != HandFightTokens::DOUBLING) {
            only_doubling = false;
        }
    }
    bool only_shield = true;
    for (const auto &token : tokens) {
        if (token.hand != HandFightTokens::NOTHING &&
            token.hand != HandFightTokens::SHIELD) {
            only_shield = false;
        }
    }
    bool only_dexterity = false;
    if (tokens.size() == 1 &&
        tokens[0].hand ==
            HandFightTokens::DEXTERITY) {
        only_dexterity = true;
    }
    return only_dexterity || only_doubling || only_shield;
}

bool Fight::check_end_round() {
    if (check_end_round_private(m_character_remaining_tokens)) {
        m_pass_character = true;
    }
    if (check_end_round_private(m_enemy_remaining_tokens)) {
        m_pass_enemy = true;
    }
    std::cout << m_pass_character << ' ' << m_pass_enemy << '\n';
    return m_pass_character && m_pass_enemy;
}

void Fight::change_turn() {
    if (m_pass_enemy) {
        m_turn = Participant::CHARACTER;
    } else if (m_pass_character) {
        m_turn = Participant::ENEMY;
    } else {
        m_turn = static_cast<Participant>(static_cast<int>(m_turn) ^ 1);
    }
}

void Fight::make_progress(
    Participant participant,
    const std::vector<TokenHandCount> &tokens,
    std::optional<TokenHandCount> dexterity_token,
    std::optional<Participant> dexterity_participant,
    std::optional<TokenHandCount> doubling_token
) {
    if (m_turn != participant) {
        throw WrongCharacterTurnException();
    }
    if (!check_combination_tokens(tokens)) {
        throw BadCombinationException();
    }
    change_turn();
    if (participant == Participant::CHARACTER) {
        switch (tokens[0].hand) {
            case (HandFightTokens::PHYSICAL_DAMAGE): {
                make_damage(Participant::ENEMY, count_damage(tokens));
                break;
            }
            case (HandFightTokens::MAGICAL_DAMAGE): {
                make_damage(Participant::ENEMY, count_damage(tokens));
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
                change_turn();
                throw BadCombinationException();
            }
        }
        for (auto token : tokens) {
            erase_token(Participant::CHARACTER, token);
        }
    } else {
        switch (tokens[0].hand) {
            case (HandFightTokens::ENEMY_DAMAGE): {
                make_damage(Participant::CHARACTER, count_damage(tokens));
                break;
            }
            case (HandFightTokens::DEXTERITY): {
                if (dexterity_participant == Participant::CHARACTER) {
                    toss_token(Participant::CHARACTER, dexterity_token.value());
                } else {
                    reverse_token(Participant::ENEMY, dexterity_token.value());
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
                change_turn();
                throw BadCombinationException();
            }
        }
        for (auto token : tokens) {
            erase_token(Participant::ENEMY, token);
        }
    }
}

HandFightTokens
Fight::toss_token(Participant participant, const TokenHandCount &token) {
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

HandFightTokens
Fight::reverse_token(Participant participant, const TokenHandCount &token) {
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

void Fight::start_round() {
    m_pass_character = false;
    m_pass_enemy = false;
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

}  // namespace runebound::fight