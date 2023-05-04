#include "product.hpp"
#include <optional>
#include "nlohmann/json.hpp"

namespace runebound::trade {

void Product::apply_product(const std::shared_ptr<character::Character> &chr
) const {
    chr->update_max_health(m_delta_max_health);
    chr->update_hand_limit(m_delta_hand_limit);
    chr->update_speed(m_delta_speed);
    if (m_fight_token != std::nullopt) {
        chr->add_fight_token(m_fight_token.value());
    }
    for (const auto &characteristic : m_delta_characteristic) {
        chr->update_characteristic(characteristic.first, characteristic.second);
    }
}

void Product::cancel_product(const std::shared_ptr<character::Character> &chr
) const {
    chr->update_max_health(-m_delta_max_health);
    chr->update_hand_limit(-m_delta_hand_limit);
    chr->update_speed(-m_delta_speed);
    if (m_fight_token != std::nullopt) {
        chr->erase_fight_token(m_fight_token.value());
    }
    for (const auto &characteristic : m_delta_characteristic) {
        chr->update_characteristic(
            characteristic.first, -characteristic.second
        );
    }
}

void to_json(nlohmann::json &json, const Product &product) {
    json["m_product_name"] = product.m_product_name;
    json["m_place_of_cell"] = product.m_place_of_cell;
    json["m_delta_characteristic"] = product.m_delta_characteristic;
    json["m_delta_max_health"] = product.m_delta_max_health;
    json["m_delta_speed"] = product.m_delta_speed;
    json["m_delta_hand_limit"] = product.m_delta_hand_limit;
    json["m_price"] = product.m_price;
    json["m_market_price"] = product.m_market_price;
    if (product.m_fight_token != std::nullopt) {
        json["m_fight_token"] = product.m_fight_token.value();
    } else {
        json["m_fight_token"] = 0;
    }
}

void from_json(const nlohmann::json &json, Product &product) {
    product.m_product_name = json["m_product_name"];
    product.m_place_of_cell = json["m_place_of_cell"];
    product.m_delta_characteristic = json["m_delta_characteristic"];
    product.m_delta_max_health = json["m_delta_max_health"];
    product.m_delta_speed = json["m_delta_speed"];
    product.m_delta_hand_limit = json["m_delta_hand_limit"];
    product.m_price = json["m_price"];
    product.m_market_price = json["m_market_price"];
    if (json["m_fight_token"] != 0) {
        product.m_fight_token = json["m_fight_token"];
    } else {
        product.m_fight_token = std::nullopt;
    }
}

}  // namespace runebound::trade