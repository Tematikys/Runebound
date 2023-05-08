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

namespace runebound {
namespace character {

void to_json(nlohmann::json &json, const Character &character);
void from_json(const nlohmann::json &json, Character &character);

enum class StateCharacter { NORMAL_GAME, FIGHT, ENEMY, CALLER, RECEIVER };
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
    std::map<Characteristic, int> m_characteristics;

    int m_action_points = 3;
    int m_max_action_points = 3;
    unsigned int m_active_card_meeting;
    unsigned int m_active_card_research;
    StandardCharacter m_standard_character = StandardCharacter::LISSA;
    StateCharacter m_current_state = StateCharacter::NORMAL_GAME;
    std::string m_name;
    std::set<unsigned int> m_cards_research;
    std::set<unsigned int> m_cards_fight;
    std::set<unsigned int> m_cards_meeting;
    std::set<std::pair<AdventureType, unsigned int>> m_trophies;

    bool m_is_in_trade = false;
    int m_max_health;
    int m_gold, m_health;
    int m_knowledge_token = 0;
    Point m_current_position;
    std::shared_ptr<::runebound::fight::Fight> m_current_fight = nullptr;
    std::shared_ptr<::runebound::fight::FightTwoPlayer>
        m_current_fight_two_player = nullptr;
    std::shared_ptr<::runebound::character::Character>
        m_current_caller_to_fight = nullptr;
    std::vector<::runebound::fight::FightToken> m_fight_tokens;
    std::set<unsigned int> m_products;

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

    [[nodiscard]] std::set<unsigned int> get_products() const {
        return m_products;
    }

    [[nodiscard]] bool check_in_trade() const {
        return m_is_in_trade;
    }

    void start_trade() {
        m_is_in_trade = true;
    }

    [[nodiscard]] bool check_caller_to_fight() const {
        return m_current_caller_to_fight != nullptr;
    }

    [[nodiscard]] std::shared_ptr<Character> get_current_caller_to_fight(
    ) const {
        return m_current_caller_to_fight;
    }

    void call_to_fight(const std::shared_ptr<Character> &caller) {
        m_current_caller_to_fight = caller;
    }

    void refuse_to_fight() {
        m_current_caller_to_fight = nullptr;
    }

    void start_fight_two_player(
        const std::shared_ptr<fight::FightTwoPlayer> &fight,
        StateCharacter status
    ) {
        m_current_fight_two_player = fight;
        m_current_state = status;
        m_current_caller_to_fight = nullptr;
    }

    void end_fight_two_player() {
        m_current_fight_two_player = nullptr;
        m_current_state = StateCharacter::NORMAL_GAME;
    }

    void end_trade() {
        m_is_in_trade = false;
    }

    void set_health(int health) {
        m_health = health;
    }

    [[nodiscard]] unsigned int get_card_fight() const {
        return *(--m_cards_fight.end());
    }

    [[nodiscard]] int get_knowledge_token() const {
        return m_knowledge_token;
    }

    void change_knowledge_token(int delta) {
        m_knowledge_token += delta;
    }

    [[nodiscard]] bool check_product(unsigned int product) const {
        return m_products.count(product) != 0;
    }

    void add_product(unsigned int product) {
        m_products.insert(product);
    }

    void erase_product(unsigned int product) {
        m_products.erase(product);
    }

    [[nodiscard]] std::set<unsigned int> get_cards(AdventureType type) const {
        if (type == AdventureType::RESEARCH) {
            return m_cards_research;
        } else if (type == AdventureType::FIGHT) {
            return m_cards_fight;
        }
        return m_cards_meeting;
    }

    [[nodiscard]] int get_characteristic(Characteristic characteristic) {
        return m_characteristics[characteristic];
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

    void start_fight_as_enemy() {
        m_current_state = StateCharacter::ENEMY;
    }

    void end_fight_as_enemy() {
        m_current_state = StateCharacter::NORMAL_GAME;
    }

    [[nodiscard]] bool check_card(AdventureType type, unsigned int card) const;

    void change_gold(int delta_gold) {
        m_gold += delta_gold;
    }

    void end_fight();

    void add_trophy(AdventureType type, unsigned int card) {
        m_trophies.insert({type, card});
    }

    void make_active_card(AdventureType type, unsigned int card) {
        if (type == AdventureType::MEETING) {
            m_active_card_meeting = card;
        }
        if (type == AdventureType::RESEARCH) {
            m_active_card_research = card;
        }
    }

    [[nodiscard]] unsigned int get_active_card_research() const {
        return m_active_card_research;
    }

    [[nodiscard]] unsigned int get_active_card_meeting() const {
        return m_active_card_meeting;
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

    [[nodiscard]] std::shared_ptr<::runebound::fight::FightTwoPlayer>
    get_current_fight_two_player() const {
        return m_current_fight_two_player;
    }

    Character(
        int gold,
        int health,
        Point current_position,
        unsigned int hand_limit,
        unsigned int speed,
        std::string name,
        std::vector<fight::FightToken> fight_tokens,
        int body,
        int intelligence,
        int spirit

    )
        : m_gold(gold),
          m_health(health),
          m_max_health(health),
          m_current_position(current_position),
          m_hand_limit(hand_limit),
          m_speed(speed),
          m_name(std::move(name)),
          m_fight_tokens(std::move(fight_tokens)) {
        m_characteristics[Characteristic::BODY] = body;
        m_characteristics[Characteristic::INTELLIGENCE] = intelligence;
        m_characteristics[Characteristic::SPIRIT] = spirit;
    }

    [[nodiscard]] std::string get_name() const {
        return m_name;
    }

    [[nodiscard]] int get_gold() const {
        return m_gold;
    }

    void relax() {
        m_health = m_max_health;
    }

    void update_max_health(int delta) {
        m_max_health += delta;
    }

    void update_speed(int delta) {
        m_speed += delta;
    }

    void update_hand_limit(int delta) {
        m_hand_limit += delta;
    }

    void add_fight_token(const fight::FightToken &token) {
        m_fight_tokens.push_back(token);
    }

    void erase_fight_token(const fight::FightToken &token) {
        m_fight_tokens.erase(
            std::find(m_fight_tokens.begin(), m_fight_tokens.end(), token)
        );
    }

    void update_characteristic(Characteristic characteristic, int delta) {
        m_characteristics[characteristic] += delta;
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

    [[nodiscard]] nlohmann::json to_json() const {
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
