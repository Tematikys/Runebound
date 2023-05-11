#include "map.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <queue>
#include "dice.hpp"

namespace runebound {
namespace map {

const std::vector<Point> &Map::get_directions(const Point &point) const {
    if (point.y % 2 == 0) {
        return directions_even_column;
    }
    return directions_odd_column;
}

bool Map::check_neighbour(const Point &lhs, const Point &rhs) const {
    auto directions = get_directions(lhs);
    for (const auto &direction : directions) {
        if (lhs + direction == rhs) {
            return true;
        }
    }
    return false;
}

std::vector<Point> Map::make_move(
    const Point &start,
    const Point &end,
    const std::vector<::runebound::dice::HandDice> &dice_roll_results,
    int count_dice
) const {
    std::map<Point, int> dist;
    std::map<Point, Point> parent;
    dist[start] = 0;
    std::queue<Point> bfs_queue;
    bfs_queue.push(start);
    parent[start] = Point(-1, -1);
    while (!bfs_queue.empty()) {
        auto current = bfs_queue.front();
        bfs_queue.pop();
        if (dist[current] > count_dice) {
            return {};
        }
        if (current == end) {
            std::vector<Point> result;
            while (current != Point(-1, -1)) {
                result.push_back(current);
                current = parent[current];
            }
            std::reverse(result.begin(), result.end());
            return result;
        }
        for (const auto &direction : get_directions(current)) {
            if (check_neighbour_in_direction(current, direction)) {
                auto new_point = get_neighbour_in_direction(current, direction);
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
                           ::runebound::dice::HandDice::MOUNTAINS_WATER)))) {
                    dist[new_point] = dist[current] + 1;
                    parent[new_point] = current;
                    bfs_queue.push(new_point);
                }
            }
        }
    }
    return {};
}

std::vector<Point> Map::check_move(
    const Point &start,
    const Point &end,
    std::vector<::runebound::dice::HandDice> dice_roll_results
) const {
    if (check_neighbour(start, end)) {
        return {start, end};
    }
    std::sort(dice_roll_results.begin(), dice_roll_results.end());
    do {
        auto result = make_move(
            start, end, dice_roll_results,
            static_cast<int>(dice_roll_results.size())
        );
        if (!result.empty()) {
            return result;
        }

    } while (std::next_permutation(
        dice_roll_results.begin(), dice_roll_results.end()
    ));
    return {};
}

void to_json(nlohmann::json &json, const Map &map) {
    json["m_map"] = map.m_map;
    json["m_size"] = map.m_size;
    json["m_rivers"] = map.m_rivers;
    json["m_towns"] = map.m_towns;
    json["m_territory_name"] = map.m_territory_name;
}

void from_json(const nlohmann::json &json, Map &map) {
    map.m_map = json["m_map"];
    map.m_rivers = json["m_rivers"];
    map.m_towns = json["m_towns"];
    map.m_size = json["m_size"];
    map.m_territory_name = json["m_territory_name"];
}

}  // namespace map
}  // namespace runebound
