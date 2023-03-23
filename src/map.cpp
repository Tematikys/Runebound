#include "map.hpp"
#include <algorithm>
#include <json.hpp>
#include <map>
#include <queue>
#include "dice.hpp"

namespace runebound {
namespace map {

void Map::make_row(
    int row,
    const std::vector<std::pair<TypeCell, int>> &elements
) {
    int column = 0;
    for (const auto &one_type : elements) {
        for (int i = 0; i < one_type.second; ++i) {
            m_map[row][column] = MapCell(one_type.first);
            column += 1;
        }
    }
}

void Map::make_cells() {
    make_row(
        0,
        std::vector{
            std::pair(TypeCell::PLAIN, 7), std::pair(TypeCell::FOREST, 6),
            std::pair(TypeCell::HILLS, 1), std::pair(TypeCell::MOUNTAINS, 1)}
    );
    make_row(
        1,
        std::vector{
            std::pair(TypeCell::FOREST, 2), std::pair(TypeCell::PLAIN, 5),
            std::pair(TypeCell::FOREST, 4), std::pair(TypeCell::TOWN, 1),
            std::pair(TypeCell::HILLS, 1), std::pair(TypeCell::MOUNTAINS, 2)}
    );
    make_row(
        2,
        std::vector{
            std::pair(TypeCell::FOREST, 2), std::pair(TypeCell::MOUNTAINS, 1),
            std::pair(TypeCell::PLAIN, 5), std::pair(TypeCell::FOREST, 1),
            std::pair(TypeCell::PLAIN, 3), std::pair(TypeCell::MOUNTAINS, 3)}
    );
    make_row(
        3,
        std::vector{
            std::pair(TypeCell::FOREST, 1), std::pair(TypeCell::PLAIN, 2),
            std::pair(TypeCell::HILLS, 1), std::pair(TypeCell::PLAIN, 8),
            std::pair(TypeCell::MOUNTAINS, 3)}
    );
    make_row(
        4,
        std::vector{
            std::pair(TypeCell::PLAIN, 5), std::pair(TypeCell::WATER, 1),
            std::pair(TypeCell::PLAIN, 2), std::pair(TypeCell::FOREST, 1),
            std::pair(TypeCell::PLAIN, 1), std::pair(TypeCell::WATER, 1),
            std::pair(TypeCell::PLAIN, 2), std::pair(TypeCell::HILLS, 2)}
    );
    make_row(
        5,
        std::vector{
            std::pair(TypeCell::PLAIN, 1), std::pair(TypeCell::HILLS, 1),
            std::pair(TypeCell::PLAIN, 2), std::pair(TypeCell::WATER, 2),
            std::pair(TypeCell::TOWN, 1), std::pair(TypeCell::PLAIN, 4),
            std::pair(TypeCell::FOREST, 2), std::pair(TypeCell::HILLS, 2)}
    );
    make_row(
        6,
        std::vector{
            std::pair(TypeCell::HILLS, 1), std::pair(TypeCell::PLAIN, 1),
            std::pair(TypeCell::FOREST, 2), std::pair(TypeCell::PLAIN, 6),
            std::pair(TypeCell::FOREST, 3), std::pair(TypeCell::HILLS, 2)}
    );
    make_row(
        7,
        std::vector{
            std::pair(TypeCell::HILLS, 1), std::pair(TypeCell::PLAIN, 4),
            std::pair(TypeCell::WATER, 1), std::pair(TypeCell::PLAIN, 3),
            std::pair(TypeCell::MOUNTAINS, 1), std::pair(TypeCell::PLAIN, 2),
            std::pair(TypeCell::HILLS, 1), std::pair(TypeCell::PLAIN, 2)}
    );
    make_row(
        8,
        std::vector{
            std::pair(TypeCell::PLAIN, 10), std::pair(TypeCell::MOUNTAINS, 1),
            std::pair(TypeCell::PLAIN, 4)}
    );
    make_row(
        9,
        std::vector{
            std::pair(TypeCell::FOREST, 1), std::pair(TypeCell::PLAIN, 10),
            std::pair(TypeCell::WATER, 1), std::pair(TypeCell::PLAIN, 3)}
    );
    make_row(
        10,
        std::vector{
            std::pair(TypeCell::FOREST, 1), std::pair(TypeCell::PLAIN, 1),
            std::pair(TypeCell::TOWN, 1), std::pair(TypeCell::HILLS, 1),
            std::pair(TypeCell::PLAIN, 3), std::pair(TypeCell::HILLS, 1),
            std::pair(TypeCell::PLAIN, 6), std::pair(TypeCell::FOREST, 1)}
    );
    make_row(
        11,
        std::vector{
            std::pair(TypeCell::PLAIN, 4), std::pair(TypeCell::HILLS, 1),
            std::pair(TypeCell::MOUNTAINS, 1), std::pair(TypeCell::PLAIN, 1),
            std::pair(TypeCell::HILLS, 1), std::pair(TypeCell::WATER, 2),
            std::pair(TypeCell::PLAIN, 3), std::pair(TypeCell::TOWN, 1),
            std::pair(TypeCell::FOREST, 1)}
    );
    make_row(
        12,
        std::vector{
            std::pair(TypeCell::PLAIN, 3), std::pair(TypeCell::HILLS, 2),
            std::pair(TypeCell::MOUNTAINS, 2), std::pair(TypeCell::HILLS, 3),
            std::pair(TypeCell::PLAIN, 5)}
    );
    make_row(
        13,
        std::vector{
            std::pair(TypeCell::PLAIN, 3), std::pair(TypeCell::HILLS, 2),
            std::pair(TypeCell::MOUNTAINS, 3), std::pair(TypeCell::HILLS, 1),
            std::pair(TypeCell::MOUNTAINS, 1), std::pair(TypeCell::PLAIN, 5)}
    );
    make_row(
        14,
        std::vector{
            std::pair(TypeCell::PLAIN, 3), std::pair(TypeCell::HILLS, 2),
            std::pair(TypeCell::MOUNTAINS, 3), std::pair(TypeCell::HILLS, 1),
            std::pair(TypeCell::MOUNTAINS, 1), std::pair(TypeCell::PLAIN, 5)}
    );
}

void Map::make_map() {
    m_map.resize(m_size, std::vector<MapCell>(m_size));
    make_cells();
}

const std::vector<Point> &Map::get_directions(int x) const {
    if (x % 2 == 0) {
        return directions_even_row;
    }
    return directions_odd_row;
}

bool Map::make_move(
    const Point &start,
    const Point &end,
    const std::vector<::runebound::dice::HandDice> &dice_roll_results,
    int count_dice
) const {
    std::map<Point, int> dist;
    dist[start] = 0;
    std::queue<Point> bfs_queue;
    bfs_queue.push(start);
    while (!bfs_queue.empty()) {
        auto current = bfs_queue.front();
        bfs_queue.pop();
        if (dist[current] > count_dice) {
            return false;
        }
        if (current == end) {
            return true;
        }
        for (const auto &direction : get_directions(current.x)) {
            if (check_neighbour(current, direction)) {
                auto new_point = get_neighbour(current, direction);
                if (!dist.count(new_point) &&
                        (get_cell_map(new_point).check_road() ||
                         (!check_river(current, new_point) &&
                          ::runebound::dice::check_hand_dice(
                              get_cell_map(new_point).get_type_cell(),
                              dice_roll_results[dist[current]]
                          ))) ||
                    (check_river(current, new_point) &&
                     (dice_roll_results[dist[current]] ==
                          ::runebound::dice::HandDice::JOKER ||
                      dice_roll_results[dist[current]] ==
                          ::runebound::dice::HandDice::MOUNTAINS_WATER))) {
                    dist[new_point] = dist[current] + 1;
                    bfs_queue.push(new_point);
                }
            }
        }
    }
    return false;
}

bool Map::check_move(
    const Point &start,
    const Point &end,
    std::vector<::runebound::dice::HandDice> dice_roll_results
) const {
    do {
        if (make_move(
                start, end, dice_roll_results,
                static_cast<int>(dice_roll_results.size())
            )) {
            return true;
        }
    } while (std::next_permutation(
        dice_roll_results.begin(), dice_roll_results.end()
    ));
    return false;
}

void to_json(nlohmann::json &json, const Map &map) {
    json["m_map"] = map.m_map;
    json["m_size"] = map.m_size;
    json["m_rivers"] = map.m_rivers;
    json["directions_odd_row"] = map.directions_odd_row;
    json["directions_even_row"] = map.directions_even_row;
}

void from_json(const nlohmann::json &json, Map &map) {
    map.m_map = json["m_map"];
}

}  // namespace map
}  // namespace runebound
