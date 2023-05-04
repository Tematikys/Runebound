#ifndef GAME_HPP_
#define GAME_HPP_

#include <map>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <utility>
#include <vector>
#include "card_fight.hpp"
#include "card_meeting.hpp"
#include "card_research.hpp"
#include "character.hpp"
#include "fight.hpp"
#include "map.hpp"
#include "runebound_fwd.hpp"
#include "skill_card.hpp"
#include "tokens.hpp"
#include "product.hpp"
#include "shop.hpp"

namespace runebound {
const int DECK_SIZE = 15;

namespace game {
// void to_json(nlohmann::json &json, const Game &game);
// void from_json(const nlohmann::json &json, Game &game);

struct WrongCharacterTurnException : std::runtime_error {
    WrongCharacterTurnException()
        : std::runtime_error("Wrong character's turn") {
    }
};

struct NoCardException : std::runtime_error {
    NoCardException() : std::runtime_error("This card does not exist") {
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

struct BadOutcomeException : std::runtime_error {
    BadOutcomeException() : std::runtime_error("You can't get that outcome.") {
    }
};

struct WrongCellException : std::runtime_error {
    WrongCellException()
        : std::runtime_error("You are not in the correct territory space.") {
    }
};

struct Game {
private:
    ::runebound::map::Map m_map;
    std::vector<std::shared_ptr<::runebound::character::Character>>
        m_characters;
    std::vector<unsigned int> m_card_deck_research, m_card_deck_fight,
        m_card_deck_skill, m_card_deck_meeting, m_remaining_products;
    std::map<::runebound::token::Token, unsigned int> m_tokens;
    unsigned int m_turn = 0;
    unsigned int m_count_players = 0;

    std::vector<dice::HandDice> m_last_dice_movement_result;
    std::vector<dice::HandDice> m_last_dice_relax_result;
    std::vector<dice::HandDice> m_last_dice_research_result;
    std::vector<unsigned int> m_last_characteristic_check;

    std::vector<cards::CardResearch> m_all_cards_research;
    std::vector<cards::CardFight> m_all_cards_fight;
    std::vector<cards::CardMeeting> m_all_cards_meeting;
    std::vector<cards::SkillCard> m_all_skill_cards;
    std::vector<trade::Product> m_all_products;

    std::set<character::StandardCharacter> m_remaining_standard_characters = {
        character::StandardCharacter::LISSA,
        character::StandardCharacter::CORBIN,
        character::StandardCharacter::ELDER_MOK,
        character::StandardCharacter::LAUREL_FROM_BLOODWOOD,
        character::StandardCharacter::LORD_HAWTHORNE,
        character::StandardCharacter::MASTER_THORN};

    std::map <Point, std::set<unsigned int>> m_shops;

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

    void generate_all_skill_cards();
    void generate_all_cards_fight();
    void generate_all_cards_research();
    void generate_all_cards_meeting();
    void generate_all_products();
    void generate_all_shops();

    void generate_all() {
        generate_all_cards_research();
        generate_all_cards_fight();
        generate_all_cards_meeting();
        generate_all_skill_cards();
        generate_all_products();
        generate_all_shops();
    }

    bool check_characteristic_private(
        int number_attempts,
        Characteristic characteristic
    );

public:
    Game() {
        generate_all();
    };

    [[nodiscard]] cards::CardResearch get_card_research(unsigned int card
    ) const {
        return m_all_cards_research[card];
    }

    [[nodiscard]] cards::CardMeeting get_card_meeting(unsigned int card) const {
        return m_all_cards_meeting[card];
    }

    [[nodiscard]] cards::CardFight get_card_fight(unsigned int card) const {
        return m_all_cards_fight[card];
    }

    [[nodiscard]] std::vector<Point> get_territory_cells(
        const std::string territory
    ) {
        return m_map.get_territory_cells(territory);
    }

    void take_token(const std::shared_ptr<character::Character> &chr);

    [[nodiscard]] std::vector<dice::HandDice> get_last_dice_movement_result(
    ) const {
        return m_last_dice_movement_result;
    }

    [[nodiscard]] std::vector<dice::HandDice> get_last_dice_research_result(
    ) const {
        return m_last_dice_research_result;
    }

    [[nodiscard]] std::vector<dice::HandDice> get_last_dice_relax_result(
    ) const {
        return m_last_dice_relax_result;
    }

    void start_next_character_turn(
        const std::shared_ptr<character::Character> &chr
    ) {
        check_turn(chr);
        m_last_dice_movement_result.clear();
        m_last_dice_research_result.clear();
        m_last_dice_relax_result.clear();
        m_last_characteristic_check.clear();
        m_turn = (m_turn + 1) % m_count_players;
        m_characters[m_turn]->restore_action_points();
    }

    std::vector<dice::HandDice> throw_movement_dice(
        const std::shared_ptr<character::Character> &chr
    ) {
        check_turn(chr);
        check_sufficiency_action_points(1);
        m_last_dice_movement_result =
            ::runebound::dice::get_combination_of_dice(chr->get_speed());
        chr->update_action_points(-1);
        return m_last_dice_movement_result;
    }

    std::vector<dice::HandDice> throw_research_dice(
        const std::shared_ptr<character::Character> &chr
    ) {
        check_turn(chr);
        m_last_dice_research_result =
            ::runebound::dice::get_combination_of_dice(chr->get_speed());
        return m_last_dice_research_result;
    }

    std::vector<dice::HandDice> throw_relax_dice(
        const std::shared_ptr<character::Character> &chr
    ) {
        check_turn(chr);
        m_last_dice_relax_result =
            ::runebound::dice::get_combination_of_dice(5);
        return m_last_dice_relax_result;
    }

    std::vector<dice::HandDice> throw_dice(
        const std::shared_ptr<character::Character> &chr
    ) {
        check_turn(chr);
        m_last_dice_movement_result =
            ::runebound::dice::get_combination_of_dice(chr->get_speed());
        return m_last_dice_movement_result;
    }

    void end_fight(const std::shared_ptr<character::Character> &chr);

    void relax(std::shared_ptr<character::Character> chr);

    [[nodiscard]] std::vector<::runebound::character::Character>
    get_character_without_shared_ptr() const {
        std::vector<::runebound::character::Character> result;
        for (const auto &character : m_characters) {
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
        const std::vector<::runebound::fight::FightToken> &tokens
    ) {
        m_characters.emplace_back(
            std::make_shared<::runebound::character::Character>(
                ::runebound::character::Character(
                    gold, health, current, hand_limit, speed, std::move(name),
                    tokens, 0, 0, 0
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

    [[nodiscard]] Point get_position_character(
        const std::shared_ptr<character::Character> &chr
    ) const;

    [[nodiscard]] unsigned int get_turn() const {
        return m_turn;
    }

    std::vector<Point> make_move(
        const std::shared_ptr<character::Character> &chr,
        const Point &point,
        std::vector<::runebound::dice::HandDice> &dice_roll_results
    );

    void start_card_execution(
        const std::shared_ptr<character::Character> &chr,
        unsigned int card,
        AdventureType type
    ) {
        check_turn(chr);
        if (!chr->check_card(type, card)) {
            throw NoCardException();
        }

        if (type == AdventureType::RESEARCH) {
            auto required_cells = m_map.get_territory_cells(
                m_all_cards_research[card].get_required_territory()
            );
            if (std::find(
                    required_cells.begin(), required_cells.end(),
                    chr->get_position()
                ) == required_cells.end()) {
                throw WrongCellException();
            }
        }
        chr->make_active_card(type, card);
    }

    [[nodiscard]] std::vector<std::size_t> get_possible_outcomes(
        const std::shared_ptr<character::Character> &chr
    );

    void complete_card_research(
        const std::shared_ptr<character::Character> &chr,
        int desired_outcome = -1
    );

    bool check_characteristic(
        const std::shared_ptr<character::Character> &chr,
        unsigned int card,
        cards::OptionMeeting option
    );

    [[nodiscard]] std::vector<unsigned int> get_last_characteristic_check(
    ) const {
        return m_last_characteristic_check;
    }

    void check_characteristic_additionally(
        const std::shared_ptr<character::Character> &chr,
        unsigned int card
    ) {
        // TODO, card skill when they will be in the game not only as a
        // characteristic checking card
    }

    // friend void to_json(nlohmann::json &json, const Game &game);
    // friend void from_json(const nlohmann::json &json, Game &game);
};
}  // namespace game
}  // namespace runebound
#endif  // GAME_HPP_
