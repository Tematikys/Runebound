#ifndef CHARACTER_HPP_
#define CHARACTER_HPP_

#include <fstream>
#include <map>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <set>
#include <string>
#include <vector>
#include "card_adventure.hpp"
#include "fight_token.hpp"
#include "point.hpp"
#include "runebound_fwd.hpp"
#include "tokens.hpp"

// #include "fight.hpp"

namespace runebound {
namespace character {

void to_json(nlohmann::json &json, const Character &character);
void from_json(const nlohmann::json &json, Character &character);

enum class StateCharacter { NORMAL_GAME, FIGHT };
enum class StandardCharacter {
    LISSA,
    CORBIN,
    ELDER_MOK,
    LAUREL_FROM_BLOODWOOD,
    LORD_HAWTHORNE,
    MASTER_THORN
};

struct Character {
private:
    unsigned int m_hand_limit, m_speed;
    int m_action_points = 3;
    int m_max_action_points = 3;
    StandardCharacter m_standard_character = StandardCharacter::LISSA;
    StateCharacter m_current_state = StateCharacter::NORMAL_GAME;
    std::string m_name;
    std::set<unsigned int> m_cards_research;
    std::set<unsigned int> m_cards_fight;
    std::set<std::pair<AdventureType, unsigned int>> m_trophies;

    std::map<runebound::token::Token, int> m_tokens;
    int m_max_health;
    int m_gold, m_health;
    Point m_current_position;
    std::shared_ptr<::runebound::fight::Fight> m_current_fight = nullptr;
    std::vector<::runebound::fight::FightToken> m_fight_tokens;

    void load_character_from_file(const std::string &file);

public:
    Character()
        : m_gold(0),
          m_health(0),
          m_hand_limit(0),
          m_speed(0),
          m_max_health(0),
          m_max_action_points(0),
          m_current_position(0, 0) {
    }

    explicit Character(
        int gold,
        int health,
        Point current_position,
        unsigned int hand_limit,
        unsigned int speed,
        std::string name
    )
        : m_gold(gold),
          m_health(health),
          m_max_health(health),
          m_current_position(current_position),
          m_hand_limit(hand_limit),
          m_speed(speed),
          m_name(std::move(name)) {
    }

    explicit Character(const StandardCharacter &chr);

    [[nodiscard]] Point get_position() const {
        return m_current_position;
    }

    [[nodiscard]] unsigned int get_card_fight() const {
        return *(--m_cards_fight.end());
    }

    void set_position(const Point &new_position) {
        m_current_position = new_position;
    }

    [[nodiscard]] StateCharacter get_state() const {
        return m_current_state;
    }

    void start_fight(std::shared_ptr<::runebound::fight::Fight> fight) {
        m_current_state = StateCharacter::FIGHT;
        m_current_fight = std::move(fight);
    }

    void change_gold(int delta_gold) {
        m_gold += delta_gold;
    }

    void end_fight();

    void add_trophy(AdventureType type, unsigned int card) {
        m_trophies.insert({type, card});
    }

    [[nodiscard]] std::set<std::pair<AdventureType, unsigned int>> get_trophies(
    ) const {
        return m_trophies;
    }

    [[nodiscard]] std::set<unsigned int> get_cards_fight() const {
        return m_cards_fight;
    }

    [[nodiscard]] std::shared_ptr<::runebound::fight::Fight> get_current_fight(
    ) const {
        return m_current_fight;
    }

    Character(
        int gold,
        int health,
        Point current_position,
        unsigned int hand_limit,
        unsigned int speed,
        std::string name,
        const std::vector<fight::FightToken> fight_tokens
    )
        : m_gold(gold),
          m_health(health),
          m_max_health(health),
          m_current_position(current_position),
          m_hand_limit(hand_limit),
          m_speed(speed),
          m_name(std::move(name)),
          m_fight_tokens(fight_tokens) {
    }

    [[nodiscard]] std::string get_name() const {
        return m_name;
    }

    void relax() {
        m_health = m_max_health;
    }

    void update_action_points(int delta) {
        m_action_points += delta;
    }

    void restore_action_points() {
        m_action_points = m_max_action_points;
    }

    void update_health(int delta) {
        m_health += delta;
    }

    [[nodiscard]] int get_health() const {
        return m_health;
    }

    [[nodiscard]] std::vector<::runebound::fight::FightToken> get_fight_token(
    ) const {
        return m_fight_tokens;
    }

    [[nodiscard]] unsigned int get_speed() const {
        return m_speed;
    }

    [[nodiscard]] unsigned int get_action_points() const {
        return m_action_points;
    }

    void add_card(AdventureType type, unsigned int card);

    void pop_card(AdventureType type, unsigned int card);

    friend void to_json(nlohmann::json &json, const Character &character);
    friend void from_json(const nlohmann::json &json, Character &character);

    nlohmann::json to_json() {
        nlohmann::json json;
        ::runebound::character::to_json(json, *this);
        return json;
    }

    [[nodiscard]] StandardCharacter get_standard_character() const {
        return m_standard_character;
    }

    static Character from_json(const nlohmann::json &json) {
        Character chr;
        ::runebound::character::from_json(json, chr);
        return chr;
    }
};
}  // namespace character
}  // namespace runebound
#endif  // CHARACTER_HPP_
