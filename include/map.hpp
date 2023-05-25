#ifndef MAP_HPP_
#define MAP_HPP_

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <set>
#include <vector>
#include "dice.hpp"
#include "map_cell.hpp"
#include "point.hpp"
#include <queue>

namespace runebound::map {

const int STANDARD_SIZE = 15;

void to_json(nlohmann::json &json, const Map &map);
void from_json(const nlohmann::json &json, Map &map);

struct Map {
private:
    friend struct MapClient;
    std::vector<std::vector<MapCell>> m_map;  // [row][column]
    std::set<std::pair<Point, Point>> m_rivers;
    std::set<Point> m_towns;
    int m_size;
    std::map<std::string, std::vector<Point>> m_territory_name;
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

    void find_towns() {
        for (int row = 0; row < STANDARD_SIZE; ++row) {
            for (int column = 0; column < STANDARD_SIZE; ++column) {
                if (m_map[row][column].get_type_cell() == TypeCell::TOWN) {
                    m_towns.insert(Point(row, column));
                }
            }
        }
    }

public:
    Map() {
        nlohmann::json json;
        std::ifstream in("data/json/map/map.json");
        in >> json;
        ::runebound::map::from_json(json, *this);
    }

    Map(const Map &other)
        : m_rivers(other.m_rivers),
          m_size(other.m_size),
          m_territory_name(other.m_territory_name),
          m_towns(other.m_towns),
          directions_odd_column(other.directions_odd_column),
          directions_even_column(other.directions_even_column) {
        m_map = other.m_map;
    }

    Map(Map &&other) noexcept
        : m_map(std::move(other.m_map)),
          m_rivers(std::move(other.m_rivers)),
          m_size(other.m_size),
          m_towns(std::move(other.m_towns)),
          m_territory_name(std::move(other.m_territory_name)),
          directions_odd_column(other.directions_odd_column),
          directions_even_column(other.directions_even_column) {
    }

    Map &operator=(const Map &other) {
        m_map = other.m_map;
        return *this;
    }

    Map &operator=(Map &&other) noexcept {
        m_map = std::move(other.m_map);
        return *this;
    }

    ~Map() = default;

    Map(int size,
        std::vector<std::vector<MapCell>> map,
        std::set<std::pair<Point, Point>> rivers,
        std::map<std::string, std::vector<Point>> territory_name)
        : m_size(size),
          m_map(std::move(map)),
          m_rivers(std::move(rivers)),
          m_territory_name(std::move(territory_name)) {
        find_towns();
    }

    [[nodiscard]] std::set<Point> get_towns() const {
        return m_towns;
    }

    void make_boss(const Point &point) {
        m_map[point.x][point.y].make_boss();
    }

    void delete_boss(const Point &point) {
        m_map[point.x][point.y].delete_boss();
    }

    [[nodiscard]] bool check_neighbour(const Point &lhs, const Point &rhs)
        const;

    [[nodiscard]] std::vector<std::vector<MapCell>> get_full_map() const {
        return m_map;
    }

    [[nodiscard]] std::vector<Point> get_territory_cells(
        const std::string &territory
    ) {
        if (m_territory_name.count(territory) == 0) {
            return {};
        }
        return m_territory_name[territory];
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

    [[nodiscard]] std::vector <Point> get_neighbours(Point current) const;

    [[nodiscard]] std::vector <Point> get_possible_moves(Point start, std::vector<::runebound::dice::HandDice> dice_roll_results) const {
        std::map<Point, unsigned int> dist;
        std::sort(dice_roll_results.begin(), dice_roll_results.end());
        do {
            dist[start] = 0;
            std::queue<Point> bfs_queue;
            while (!bfs_queue.empty()) {
                auto current = bfs_queue.front();
                bfs_queue.pop();
                if (dist[current] >
                    static_cast<unsigned int>(dice_roll_results.size())) {
                    break;
                }
                for (const auto &direction : get_directions(current)) {
                    if (check_neighbour_in_direction(current, direction)) {
                        auto new_point =
                            get_neighbour_in_direction(current, direction);
                        if (!dist.count(new_point) &&
                            ((get_cell_map(new_point).check_road() ||
                              (!check_river(current, new_point) &&
                               ::runebound::dice::check_hand_dice(
                                   get_cell_map(new_point).get_type_cell(),
                                   dice_roll_results[dist[current]]
                               ))) ||
                             (check_river(current, new_point) &&
                              (dice_roll_results[dist[current]] ==
                                   ::runebound::dice::HandDice::JOKER ||
                               dice_roll_results[dist[current]] ==
                                   ::runebound::dice::HandDice::MOUNTAINS_WATER)
                             ))) {
                            if (dist[current] + 1 <=
                                static_cast<unsigned int>(
                                    dice_roll_results.size()
                                )) {
                                dist[new_point] = dist[current] + 1;
                                bfs_queue.push(new_point);
                            }
                        }
                    }
                }
            }


        } while (std::next_permutation(
            dice_roll_results.begin(), dice_roll_results.end()
        ));
        std::vector <Point> possible_moves;
        for (const auto &move : dist) {
            if (move.first != start) {
                possible_moves.push_back(move.first);
            }
        }
        return possible_moves;
    }

};
}  // namespace runebound::map

#endif  // MAP_HPP_
