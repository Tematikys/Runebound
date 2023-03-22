#ifndef MAP_HPP_
#define MAP_HPP_

#include <iostream>
#include <json_fwd.hpp>
#include <set>
#include <vector>
#include "dice.hpp"
#include "map_cell.hpp"
#include "point.hpp"

namespace runebound {
namespace map {
const int StandartHeight = 5, StandartWidth = 5;

unsigned int make_river_index(int x1, int y1, int x2, int y2);

void to_json(nlohmann::json &json, const Map &map);
void from_json(const nlohmann::json &json, Map &map);

struct Map {
private:
    std::vector<std::vector<MapCell>> m_map;  // [row][column]
    std::set<std::pair<Point, Point>> m_rivers;
    const int m_height, m_width;
    const std::vector<Point> directions_odd_row{{0, -1}, {1, 0},  {0, 1},
                                                {-1, 1}, {-1, 0}, {-1, -1}};
    const std::vector<Point> directions_even_row{{1, -1}, {1, 0},  {1, 1},
                                                 {0, 1},  {-1, 0}, {0, -1}};

    [[nodiscard]] bool make_move(
        const Point &start,
        const Point &end,
        const std::vector<::runebound::dice::HandDice> &dice_roll_results,
        int count_dice
    ) const;

    void make_map(std::vector<TypeCell> cells) {
        m_map.resize(m_height);
        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                m_map[i].push_back(MapCell(cells[i * m_height + j]));
            }
        }
    }

public:
    [[nodiscard]] int get_height() const {
        return m_height;
    }

    [[nodiscard]] int get_width() const {
        return m_width;
    }

    Map() : m_width(StandartWidth), m_height(StandartHeight) {
        std::vector<TypeCell> cells = {
            TypeCell::FOREST,    TypeCell::FOREST,    TypeCell::PLAIN,
            TypeCell::WATER,     TypeCell::WATER,     TypeCell::FOREST,
            TypeCell::FOREST,    TypeCell::PLAIN,     TypeCell::WATER,
            TypeCell::WATER,     TypeCell::PLAIN,     TypeCell::PLAIN,
            TypeCell::PLAIN,     TypeCell::PLAIN,     TypeCell::PLAIN,
            TypeCell::HILLS,     TypeCell::HILLS,     TypeCell::PLAIN,
            TypeCell::MOUNTAINS, TypeCell::MOUNTAINS, TypeCell::HILLS,
            TypeCell::HILLS,     TypeCell::PLAIN,     TypeCell::MOUNTAINS,
            TypeCell::MOUNTAINS};
        make_map(cells);

        m_rivers.insert({Point(0, 2), Point(1, 1)});
        m_rivers.insert({Point(1, 1), Point(0, 2)});
        m_rivers.insert({Point(0, 2), Point(1, 2)});
        m_rivers.insert({Point(1, 2), Point(0, 2)});
        m_rivers.insert({Point(1, 3), Point(0, 2)});
        m_rivers.insert({Point(0, 2), Point(1, 3)});
        m_map[2][0].make_token(runebound::AdventureType::FIGHT);
        m_map[2][4].make_token(runebound::AdventureType::MEETING);
        m_map[4][2].make_token(runebound::AdventureType::RESEARCH);
    }

    Map(const Map &other)
        : m_rivers(other.m_rivers),
          m_height(other.m_height),
          m_width(other.m_width),
          directions_odd_row(other.directions_odd_row),
          directions_even_row(other.directions_even_row) {
        m_map = other.m_map;
    }

    Map(Map &&other) noexcept
        : m_map(std::move(other.m_map)),
          m_rivers(std::move(other.m_rivers)),
          m_height(other.m_height),
          m_width(other.m_width),
          directions_odd_row(other.directions_odd_row),
          directions_even_row(other.directions_even_row) {
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

    Map(int width, int height) : m_height(height), m_width(width) {
    }

    [[nodiscard]] MapCell get_cell_map(const Point &point) const {
        return m_map[point.x][point.y];
    }

    [[nodiscard]] const std::vector<Point> &get_direction(int x) const;

    [[nodiscard]] Point
    get_neighbour(const Point &point, const Point &direction) const {
        return {point.x + direction.x, point.y + direction.y};
    }

    [[nodiscard]] bool
    check_neighbour(const Point &point, const Point &direction) const {
        return (point.x + direction.x >= 0) &&
               (point.x + direction.x < m_width) &&
               (point.y + direction.y >= 0) &&
               (point.y + direction.y < m_height);
    }

    bool check_river(const Point &lhs_point, const Point &rhs_point) const {
        return m_rivers.find({lhs_point, rhs_point}) != m_rivers.end();
    }

    [[nodiscard]] bool
    check_hand_dice(const Point &point, ::runebound::dice::HandDice dice) const;

    [[nodiscard]] bool check_move(
        const Point &start,
        const Point &end,
        std::vector<::runebound::dice::HandDice> dice_roll_results
    ) const;

    friend void to_json(nlohmann::json &json, const Map &map);
    friend void from_json(const nlohmann::json &json, Map &map);
};
}  // namespace map
}  // namespace runebound
#endif  // MAP_HPP_
