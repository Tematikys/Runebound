#ifndef GAME_HPP_
#define GAME_HPP_

#include <map>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <utility>
#include <vector>
#include "card_research.hpp"
#include "character.hpp"
#include "fight.hpp"
#include "map.hpp"
#include "runebound_fwd.hpp"
#include "tokens.hpp"

namespace runebound {
const int DECK_SIZE = 60;

namespace game {
// void to_json(nlohmann::json &json, const Game &game);
// void from_json(const nlohmann::json &json, Game &game);

struct WrongCharacterTurnException : std::runtime_error {
    WrongCharacterTurnException()
        : std::runtime_error("Wrong character's turn") {
    }
};

struct NotEnoughActionPointsException : std::runtime_error {
    NotEnoughActionPointsException()
        : std::runtime_error("Not enough action points") {
    }
};

struct InaccessibleMoveException : std::runtime_error {
    InaccessibleMoveException() : std::runtime_error("Can't make this move") {
    }
};

struct BackSideTokenException : std::runtime_error {
    BackSideTokenException() : std::runtime_error("Back side token") {
    }
};

struct NoTokenException : std::runtime_error {
    NoTokenException() : std::runtime_error("No token in this cell") {
    }
};

struct CharacterAlreadySelected : std::runtime_error {
    CharacterAlreadySelected()
        : std::runtime_error("This character is already selected") {
    }
};

struct Game {
private:
    ::runebound::map::Map m_map;
    std::vector<std::shared_ptr<::runebound::character::Character>>
        m_characters;
    std::vector<unsigned int> m_card_deck_research;
    std::map<::runebound::token::Token, unsigned int> m_tokens;
    unsigned int m_turn = 0;
    unsigned int m_count_players = 0;
    std::vector<unsigned int> m_indexes_card_research;
    const std::vector<cards::CardResearch> ALL_CARDS_RESEARCH;
    std::set<character::StandardCharacter> m_remaining_standard_characters = {
        character::StandardCharacter::LISSA,
        character::StandardCharacter::CORBIN,
        character::StandardCharacter::ELDER_MOK,
        character::StandardCharacter::LAUREL_FROM_BLOODWOOD,
        character::StandardCharacter::LORD_HAWTHORNE,
        character::StandardCharacter::MASTER_THORN};

    std::vector<cards::CardResearch> generate_all_cards_research();

    template <typename T>
    void pop_element_from_vector(T element, std::vector<T> &vec) {
        for (std::size_t i = 0; i < vec.size(); ++i) {
            if (vec[i] == element) {
                std::swap(vec[i], vec.back());
                break;
            }
        }
        vec.pop_back();
    }

    void check_turn(const std::shared_ptr<character::Character> &chr) {
        if (chr->get_name() != m_characters[m_turn]->get_name()) {
            throw WrongCharacterTurnException();
        }
    }

    void check_sufficiency_action_points(int necessary_action_points) {
        if (m_characters[m_turn]->get_action_points() <
            necessary_action_points) {
            throw NotEnoughActionPointsException();
        }
    }

public:
    Game() : ALL_CARDS_RESEARCH(std::move(generate_all_cards_research())) {
        m_card_deck_research.resize(DECK_SIZE);
        for (int i = 0; i < DECK_SIZE; ++i) {
            m_card_deck_research[i] = i;
        }
    };

    Game &operator=(const Game &other) {
        m_map = other.m_map;
        m_characters = other.m_characters;
        m_card_deck_research = other.m_card_deck_research;
        m_tokens = other.m_tokens;
        m_turn = other.m_turn;
        return *this;
    }

    void start_next_character_turn() {
        m_turn = (m_turn + 1) % m_count_players;
        m_characters[m_turn]->restore_action_points();
    }

    void relax(std::shared_ptr<character::Character> chr);

    [[nodiscard]] const std::vector<std::shared_ptr<::runebound::character::Character>> &get_characters() const {
        return m_characters;
    }

    [[nodiscard]] const std::vector<::runebound::character::Character> get_character_without_shared_ptr() const {
        std::vector<::runebound::character::Character> result;
        for (const auto& character : m_characters){
            result.push_back(*character);
        }
        return result;
    }


    [[nodiscard]] std::set<::runebound::character::StandardCharacter>
    get_remaining_standard_characters() const {
        return m_remaining_standard_characters;
    }

    std::shared_ptr<::runebound::character::Character> make_character(
        int gold,
        int health,
        const Point &current,
        unsigned int hand_limit,
        unsigned int speed,
        std::string name,
        const std::vector<::runebound::fight::FightToken> tokens
    ) {
        m_characters.emplace_back(
            std::make_shared<::runebound::character::Character>(
                ::runebound::character::Character(
                    gold, health, current, hand_limit, speed, std::move(name),
                    tokens
                )
            )
        );
        m_count_players += 1;
        return m_characters.back();
    }

    std::shared_ptr<::runebound::character::Character> make_character(
        const ::runebound::character::StandardCharacter &name
    );

    [[nodiscard]] int get_map_size() const {
        return m_map.get_size();
    }

    [[nodiscard]] ::runebound::map::Map get_map() const {
        return m_map;
    }

    void reverse_token(std::shared_ptr<character::Character> chr);

    [[nodiscard]] Point get_position_character(
        const std::shared_ptr<character::Character> &chr
    ) const;

    [[nodiscard]] unsigned int get_turn() const {
        return m_turn;
    }

    void check_and_get_card_adventure_because_of_token(
        std::shared_ptr<character::Character> chr
    );

    [[nodiscard]] std::vector<::runebound::dice::HandDice> throw_camping_dice(
        const std::shared_ptr<character::Character> &chr
    ) const {
        return ::runebound::dice::get_combination_of_dice(chr->get_speed());
    }

    std::vector<Point> make_move(
        const std::shared_ptr<character::Character> &chr,
        const Point &point,
        std::vector<::runebound::dice::HandDice> &dice_roll_results
    );

    // friend void to_json(nlohmann::json &json, const Game &game);
    // friend void from_json(const nlohmann::json &json, Game &game);
};
}  // namespace game
}  // namespace runebound
#endif  // GAME_HPP_
