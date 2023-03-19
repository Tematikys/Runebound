#ifndef FIELD_CELL_HPP_
#define FIELD_CELL_HPP_


#include <json_fwd.hpp>

#include <memory>
#include <utility>
#include <vector>
#include "runebound_fwd.hpp"

namespace runebound {
namespace map {

const int COUNT_NEIGHBOUR = 6;


enum class TypeCell { WATER, FOREST, MOUNTAINS, HILLS, PLAIN, TOWN };


enum class Neighbour {
    TOPRIGHT,
    RIGHT,
    BOTTOMRIGHT,
    BOTTOMLEFT,
    LEFT,
    TOPLEFT
};

void to_json(nlohmann::json &json, const MapCell &map_cell);
void from_json(const nlohmann::json &json, MapCell &map_cell);

struct MapCell {
public:
    MapCell() = default;


    MapCell(TypeCell type_cell) : m_type_cell(type_cell) {
        m_token = runebound::AdventureType::NOTHING;
        m_side_token = runebound::Side::FRONT;
    }

    void make_token(runebound::AdventureType token) {
        m_token = token;
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

    [[nodiscard]] bool check_road() const {
        return m_road;
    }

    [[nodiscard]] std::string get_special_type_cell() const {
        return m_special_type_cell;
    }

    friend void to_json(nlohmann::json &json, const MapCell &map_cell);
    friend void from_json(const nlohmann::json &json, MapCell &map_cell);

private:
    TypeCell m_type_cell;
    bool m_road;
    std::string m_special_type_cell;

    runebound::AdventureType m_token;
    runebound::Side m_side_token;
};

}  // namespace map
}  // namespace runebound
#endif  // FIELD_CELL_HPP_
