#ifndef CHARACTER_CLIENT_HPP_
#define CHARACTER_CLIENT_HPP_

#include <map>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <vector>
#include "runebound_fwd.hpp"

namespace runebound::character {

void to_json(nlohmann::json &json, const CharacterClient &character);
void from_json(const nlohmann::json &json, CharacterClient &character);

struct CharacterClient {
public:
    unsigned int m_hand_limit;
    unsigned int m_current_action_points = 3;
    unsigned int m_max_action_points = 3;
    int m_max_health;
    int m_current_health;
    std::string m_name;
    std::vector<unsigned int> m_cards;

    friend void to_json(nlohmann::json &json, const CharacterClient &character);
    friend void
    from_json(const nlohmann::json &json, CharacterClient &character);
};

}  // namespace runebound::character

#endif  // CHARACTER_CLIENT_HPP_
