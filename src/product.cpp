#include "product.hpp"
#include <optional>
#include "nlohmann/json.hpp"

namespace runebound::trade {
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