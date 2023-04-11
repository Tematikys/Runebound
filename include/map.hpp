#ifndef MAP_HPP_
#define MAP_HPP_

#include <iostream>
#include <nlohmann/json_fwd.hpp>
#include <set>
#include <vector>
#include "dice.hpp"
#include "map_cell.hpp"
#include "point.hpp"

namespace runebound {
namespace map {

const int STANDARD_SIZE = 15;

void to_json(nlohmann::json &json, const Map &map);
void from_json(const nlohmann::json &json, Map &map);

struct Map {
private:
    std::vector<std::vector<MapCell>> m_map;  // [row][column]
    std::set<std::pair<Point, Point>> m_rivers;
    const int m_size;
    const std::map<std::string, std::vector<Point>> m_territory_name;
    const std::vector<Point> directions_odd_column{{-1, 0}, {0, 1},  {1, 1},
                                                   {1, 0},  {1, -1}, {0, -1}};
    const std::vector<Point> directions_even_column{{-1, 0}, {-1, 1}, {0, 1},
                                                    {1, 0},  {0, -1}, {-1, -1}};

    [[nodiscard]] std::vector<Point> make_move(
        const Point &start,
        const Point &end,
        const std::vector<::runebound::dice::HandDice> &dice_roll_results,
        int count_dice
    ) const;

    bool check_neighbour(const Point &lhs, const Point &rhs) const;

public:
    Map()
        : m_size(STANDARD_SIZE),
          m_map(make_map()),
          m_territory_name(make_territory_name()) {
        make_connections_between_territory_names_and_cells(
            m_map, m_territory_name
        );
    }

    Map(const Map &other)
        : m_rivers(other.m_rivers),
          m_size(other.m_size),
          directions_odd_column(other.directions_odd_column),
          directions_even_column(other.directions_even_column) {
        m_map = other.m_map;
    }

    Map(Map &&other) noexcept
        : m_map(std::move(other.m_map)),
          m_rivers(std::move(other.m_rivers)),
          m_size(other.m_size),
          directions_odd_column(other.directions_odd_column),
          directions_even_column(other.directions_even_column) {
    }

    Map &operator=(const Map &other) {
        m_map = other.m_map;
        return *this;
    }

    Map &operator=(Map &&other) noexcept {
        m_map = other.m_map;
        return *this;
    }

    ~Map() = default;

    [[nodiscard]] std::vector<std::vector<MapCell>> get_full_map() const {
        return m_map;
    }

    [[nodiscard]] MapCell get_cell_map(const Point &point) const {
        return m_map[point.x][point.y];
    }

    void reverse_token(const Point &point) {
        m_map[point.x][point.y].reverse_token();
    }

    [[nodiscard]] const std::vector<Point> &get_directions(const Point &point
    ) const;

    [[nodiscard]] Point get_neighbour_in_direction(
        const Point &point,
        const Point &direction
    ) const {
        return {point.x + direction.x, point.y + direction.y};
    }

    [[nodiscard]] bool check_neighbour_in_direction(
        const Point &point,
        const Point &direction
    ) const {
        return (point.x + direction.x >= 0) &&
               (point.x + direction.x < m_size) &&
               (point.y + direction.y >= 0) && (point.y + direction.y < m_size);
    }

    bool check_river(const Point &lhs_point, const Point &rhs_point) const {
        return m_rivers.find({lhs_point, rhs_point}) != m_rivers.end();
    }

    [[nodiscard]] std::set<std::pair<Point, Point>> get_rivers() const {
        return m_rivers;
    }

    [[nodiscard]] int get_size() const {
        return m_size;
    }

    [[nodiscard]] std::vector<Point> check_move(
        const Point &start,
        const Point &end,
        std::vector<::runebound::dice::HandDice> dice_roll_results
    ) const;

    friend void to_json(nlohmann::json &json, const Map &map);
    friend void from_json(const nlohmann::json &json, Map &map);

    nlohmann::json to_json() {
        nlohmann::json json;
        ::runebound::map::to_json(json, *this);
        return json;
    }

    static Map from_json(const nlohmann::json &json) {
        Map map;
        ::runebound::map::from_json(json, map);
        return map;
    }
};
}  // namespace map
}  // namespace runebound
#endif  // MAP_HPP_
