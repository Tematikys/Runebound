#include "fight.hpp"
#include <vector>
#include "game.hpp"

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
            auto harm = std::min(m_enemy_remaining_tokens[i].count, damage);
            damage -= harm;
            m_enemy_remaining_tokens[i].count -= harm;
            if (m_enemy_remaining_tokens[i].count == 0) {
                shields.push_back(m_enemy_remaining_tokens[i]);
            }
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
                {character_token, character_token.first,
                 character_token.first_count}
            );
        } else {
            m_character_remaining_tokens.push_back(
                {character_token, character_token.second,
                 character_token.second_count}
            );
        }
    }
    for (auto &enemy_token : enemy_tokens) {
        if (rng() % 2 == 0) {
            m_enemy_remaining_tokens.push_back(
                {enemy_token, enemy_token.first, enemy_token.first_count}
            );
        } else {
            m_enemy_remaining_tokens.push_back(
                {enemy_token, enemy_token.second, enemy_token.second_count}
            );
        }
    }
}

void Fight::make_doubling_private(
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

bool Fight::check_end_round_private(const std::vector<TokenHandCount> &tokens
) const {
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
    if (tokens.size() == 1 && tokens[0].hand == HandFightTokens::DEXTERITY) {
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
    return m_pass_character && m_pass_enemy;
}

void Fight::change_turn() {
    if (check_end_round_private(m_character_remaining_tokens)) {
        m_pass_character = true;
    }
    if (check_end_round_private(m_enemy_remaining_tokens)) {
        m_pass_enemy = true;
    }
    if (m_pass_enemy) {
        m_turn = Participant::CHARACTER;
    } else if (m_pass_character) {
        m_turn = Participant::ENEMY;
    } else {
        m_turn = static_cast<Participant>(static_cast<int>(m_turn) ^ 1);
    }
}

void Fight::check_existence_token(
    Participant participant,
    TokenHandCount token
) {
    if (participant == Participant::CHARACTER) {
        if (std::count(
                m_character_remaining_tokens.begin(),
                m_character_remaining_tokens.end(), token
            ) == 0) {
            throw ForeignTokenException();
        }
    } else {
        if (std::count(
                m_enemy_remaining_tokens.begin(),
                m_enemy_remaining_tokens.end(), token
            ) == 0) {
            throw ForeignTokenException();
        }
    }
}

void Fight::make_dexterity(
    runebound::fight::Participant participant,
    const TokenHandCount &token,
    runebound::fight::TokenHandCount dexterity_token,
    runebound::fight::Participant dexterity_participant
) {
    if (m_turn != participant) {
        throw WrongCharacterTurnException();
    }
    check_existence_token(participant, token);
    change_turn();
    if (participant == Participant::CHARACTER) {
        if (dexterity_participant == Participant::CHARACTER) {
            reverse_token(Participant::CHARACTER, dexterity_token);
        } else {
            toss_token(Participant::ENEMY, dexterity_token);
        }
    } else {
        if (dexterity_participant == Participant::CHARACTER) {
            toss_token(Participant::CHARACTER, dexterity_token);
        } else {
            reverse_token(Participant::ENEMY, dexterity_token);
        }
    }
    erase_token(participant, token);
}

void Fight::make_damage(
    Participant participant,
    const std::vector<TokenHandCount> &tokens
) {
    if (m_turn != participant) {
        throw WrongCharacterTurnException();
    }
    if (!check_combination_tokens(tokens)) {
        throw BadCombinationException();
    }
    for (const auto &token : tokens) {
        check_existence_token(participant, token);
    }
    change_turn();
    if (participant == Participant::CHARACTER) {
        make_damage(Participant::ENEMY, count_damage(tokens));
    } else {
        make_damage(Participant::CHARACTER, count_damage(tokens));
    }
    std::cout << "KATYA" << m_character_remaining_tokens.size() << ' '
              << m_enemy_remaining_tokens.size() << std::endl;
    for (auto token : tokens) {
        erase_token(participant, token);
    }
    std::cout << "KATYA" << m_character_remaining_tokens.size() << ' '
              << m_enemy_remaining_tokens.size() << std::endl;
}

void Fight::make_doubling(
    Participant participant,
    const TokenHandCount &token,
    TokenHandCount doubling_token
) {
    if (m_turn != participant) {
        throw WrongCharacterTurnException();
    }
    check_existence_token(participant, token);
    change_turn();
    make_doubling_private(participant, doubling_token);
    erase_token(participant, token);
}

void Fight::make_hit(Participant participant, const TokenHandCount &token) {
    if (m_turn != participant || m_turn != Participant::ENEMY) {
        throw WrongCharacterTurnException();
    }
    check_existence_token(participant, token);
    if (token.hand != HandFightTokens::HIT) {
        throw BadCombinationException();
    }
    change_turn();
    m_enemy.make_hit();
}

HandFightTokens
Fight::toss_token(Participant participant, const TokenHandCount &token) {
    if (participant == Participant::ENEMY) {
        for (auto &enemy_token : m_enemy_remaining_tokens) {
            if (token == enemy_token) {
                if (rng() % 2 == 0) {
                    if (enemy_token.hand == enemy_token.token.second) {
                        enemy_token.hand = enemy_token.token.first;
                        enemy_token.count = enemy_token.token.first_count;
                    } else {
                        enemy_token.hand = enemy_token.token.second;
                        enemy_token.count = enemy_token.token.second_count;
                    }
                }
                return enemy_token.hand;
            }
        }
    }
    for (auto &character_token : m_character_remaining_tokens) {
        if (token == character_token) {
            if (rng() % 2 == 0) {
                if (character_token.hand == character_token.token.second) {
                    character_token.hand = character_token.token.first;
                    character_token.count = character_token.token.first_count;
                } else {
                    character_token.hand = character_token.token.second;
                    character_token.count = character_token.token.second_count;
                }
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
                    enemy_token.count = enemy_token.token.first_count;
                } else {
                    enemy_token.hand = enemy_token.token.second;
                    enemy_token.count = enemy_token.token.second_count;
                }
                return enemy_token.hand;
            }
        }
    }
    for (auto &character_token : m_character_remaining_tokens) {
        if (token == character_token) {
            if (character_token.hand == character_token.token.second) {
                character_token.hand = character_token.token.first;
                character_token.count = character_token.token.first_count;
            } else {
                character_token.hand = character_token.token.second;
                character_token.count = character_token.token.second_count;
            }
            return character_token.hand;
        }
    }
    return HandFightTokens::NOTHING;
}

void Fight::start_round() {
    if (!m_fight_started) {
        m_fight_started = true;
        if (m_enemy.check_boss()) {
            m_enemy.update_health(
                -std::min(m_character->get_knowledge_token(), 7)
            );
        }
    }
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

void to_json(nlohmann::json &json, const Fight &fight) {
    json["m_fight_started"] = fight.m_fight_started;
    json["m_turn"] = fight.m_turn;
    json["m_pass_character"] = fight.m_pass_character;
    json["m_pass_enemy"] = fight.m_pass_enemy;
    json["m_character"] = fight.m_character->get_standard_character();
    json["m_enemy"] = fight.m_enemy;
    json["m_enemy_remaining_tokens"] = fight.m_enemy_remaining_tokens;
    json["m_character_remaining_tokens"] = fight.m_character_remaining_tokens;
}

void from_json(
    const nlohmann::json &json,
    Fight &fight,
    const runebound::game::Game &game
) {
    fight.m_fight_started = json["m_fight_started"];
    fight.m_turn = json["m_turn"];
    fight.m_pass_character = json["m_pass_character"];
    fight.m_pass_enemy = json["m_pass_enemy"];
    fight.m_character = game.get_character(json["m_character"]);
    fight.m_enemy = json["m_enemy"];
    fight.m_enemy_remaining_tokens = json["m_enemy_remaining_tokens"];
    fight.m_character_remaining_tokens = json["m_character_remaining_tokens"];
}

}  // namespace runebound::fight
