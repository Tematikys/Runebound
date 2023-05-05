#ifndef PRODUCT_HPP_
#define PRODUCT_HPP_
#include "character.hpp"
#include "fight_token.hpp"
#include "map_cell.hpp"
#include "nlohmann/json_fwd.hpp"
#include "runebound_fwd.hpp"
#include "skill_card.hpp"

namespace runebound::trade {

void to_json(nlohmann::json &json, const Product &product);
void from_json(const nlohmann::json &json, Product &product);

struct Product {
private:
    std::string m_product_name;
    std::map<Characteristic, int> m_delta_characteristic;
    int m_delta_max_health = 0;
    int m_delta_speed = 0;
    int m_delta_hand_limit = 0;
    unsigned int m_price = 0;
    unsigned int m_market_price = 0;
    map::SpecialTypeCell m_place_of_cell = map::SpecialTypeCell::NOTHING;
    std::optional<fight::FightToken> m_fight_token = std::nullopt;

public:
    Product() = default;

    Product(
        std::string product_name,
        unsigned int price,
        unsigned int market_price,
        map::SpecialTypeCell place
    )
        : m_product_name(std::move(product_name)),
          m_price(price),
          m_market_price(market_price),
          m_place_of_cell(place) {
    }

    Product(
        std::string product_name,
        unsigned int price,
        fight::FightToken token,
        std::map<Characteristic, int> delta_characteristic,
        int delta_max_health,
        int delta_speed,
        int delta_hand_limit
    )
        : m_product_name(std::move(product_name)),
          m_price(price),
          m_market_price(price),
          m_fight_token(token),
          m_delta_characteristic(std::move(delta_characteristic)),
          m_delta_max_health(delta_max_health),
          m_delta_speed(delta_speed),
          m_delta_hand_limit(delta_hand_limit) {
    }

    Product(
        std::string product_name,
        unsigned int price,
        std::map<Characteristic, int> delta_characteristic,
        int delta_max_health,
        int delta_speed,
        int delta_hand_limit
    )
        : m_product_name(std::move(product_name)),
          m_price(price),
          m_market_price(price),
          m_delta_characteristic(std::move(delta_characteristic)),
          m_delta_max_health(delta_max_health),
          m_delta_speed(delta_speed),
          m_delta_hand_limit(delta_hand_limit) {
    }

    [[nodiscard]] map::SpecialTypeCell get_place_of_cell() const {
        return m_place_of_cell;
    }

    [[nodiscard]] unsigned int get_price() const {
        return m_price;
    }

    [[nodiscard]] unsigned int get_market_price() const {
        return m_market_price;
    }

    [[nodiscard]] std::string get_product_name() const {
        return m_product_name;
    }

    [[nodiscard]] std::map<Characteristic, int> get_delta_characteristic(
    ) const {
        return m_delta_characteristic;
    }

    [[nodiscard]] int get_delta_max_health() const {
        return m_delta_max_health;
    }

    [[nodiscard]] int get_delta_speed() const {
        return m_delta_speed;
    }

    [[nodiscard]] int get_delta_hand_limit() const {
        return m_delta_hand_limit;
    }

    [[nodiscard]] std::optional<fight::FightToken> get_fight_token() const {
        return m_fight_token;
    }

    void apply_product(const std::shared_ptr<character::Character> &chr) const;
    void undo_product(const std::shared_ptr<character::Character> &chr) const;

    bool operator<(const Product &other) const {
        return m_product_name < other.m_product_name;
    }

    friend void to_json(nlohmann::json &json, const Product &product);
    friend void from_json(const nlohmann::json &json, Product &product);
};
}  // namespace runebound::trade

#endif  // PRODUCT_HPP_
