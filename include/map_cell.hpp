#ifndef FIELD_CELL_HPP_
#define FIELD_CELL_HPP_

#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <string>
#include <utility>
#include <vector>
#include "runebound_fwd.hpp"

namespace runebound {
namespace map {

enum class TypeCell { WATER, FOREST, MOUNTAINS, HILLS, PLAIN, TOWN };

enum class SpecialTypeCell { SANCTUARY, FORTRESS, SETTLEMENT, NOTHING };

void to_json(nlohmann::json &json, const MapCell &map_cell);
void from_json(const nlohmann::json &json, MapCell &map_cell);

struct MapCell {
public:
    MapCell() = default;

    explicit MapCell(TypeCell type_cell) : m_type_cell(type_cell) {
        m_token = runebound::AdventureType::NOTHING;
        m_side_token = runebound::Side::FRONT;
    }

    void make_token(runebound::AdventureType token) {
        m_token = token;
        m_side_token = runebound::Side::FRONT;
    }

    void make_boss() {
        m_token = AdventureType::BOSS;
        m_side_token = runebound::Side::FRONT;
    }

    void delete_boss() {
        m_token = AdventureType::NOTHING;
    }


    void make_special_type_cell(SpecialTypeCell special_type_cell) {
        m_special_type_cell = special_type_cell;
    }

    [[nodiscard]] runebound::AdventureType get_token() const {
        return m_token;
    }

    void reverse_token() {
        m_side_token =
            static_cast<runebound::Side>(static_cast<int>(m_side_token) ^ 1);
    }

    [[nodiscard]] runebound::Side get_side_token() const {
        return m_side_token;
    }

    [[nodiscard]] TypeCell get_type_cell() const {
        return m_type_cell;
    }

    [[nodiscard]] SpecialTypeCell get_special_type_cell() const {
        return m_special_type_cell;
    }

    [[nodiscard]] bool check_road() const {
        return m_road;
    }

    void make_road() {
        m_road = true;
    }

    void make_name_territory(const std::string &name) {
        m_territory_name = name;
    }

    [[nodiscard]] std::string get_territory_name() const {
        return m_territory_name;
    }

    friend void to_json(nlohmann::json &json, const MapCell &map_cell);
    friend void from_json(const nlohmann::json &json, MapCell &map_cell);

private:
    TypeCell m_type_cell;
    SpecialTypeCell m_special_type_cell = SpecialTypeCell::NOTHING;
    bool m_road = false;
    std::string m_territory_name;
    runebound::AdventureType m_token;
    runebound::Side m_side_token;
};

}  // namespace map
}  // namespace runebound
#endif  // FIELD_CELL_HPP_
