#include "map.hpp"
#include <algorithm>
#include <json.hpp>
#include <map>
#include <queue>
#include "dice.hpp"

namespace runebound {
namespace map {

unsigned int make_river_index(int x1, int y1, int x2, int y2) {
    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    } else {
        if ((x1 == x2) && (y1 > y2)) {
            std::swap(y1, y2);
        }
    }

    int step = 64;
    return (((x1 * step) + y1) * step + x2) * step + y2;
}

const std::vector<Point> &Map::get_direction(int x) const {
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
        for (const auto &direction : get_direction(current.x)) {
            if (check_neighbour(current, direction)) {
                auto new_point = get_neighbour(current, direction);
                if (!dist.count(new_point) &&
                        (get_cell_map(new_point).check_road() ||
                         (!check_river(current, new_point) &&
                          check_hand_dice(
                              new_point, dice_roll_results[dist[current]]
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

bool Map::check_hand_dice(const Point &point, ::runebound::dice::HandDice dice)
    const {
    switch (get_cell_map(point).get_type_cell()) {
        case (TypeCell::WATER):
            return dice == ::runebound::dice::HandDice::MOUNTAINS_WATER ||
                   dice == ::runebound::dice::HandDice::JOKER;
        case (TypeCell::FOREST):
            return dice == ::runebound::dice::HandDice::FOREST_HILLS ||
                   dice == ::runebound::dice::HandDice::PLAIN_FOREST ||
                   dice == ::runebound::dice::HandDice::JOKER;
        case (TypeCell::MOUNTAINS):
            return dice == ::runebound::dice::HandDice::MOUNTAINS_WATER ||
                   dice == ::runebound::dice::HandDice::JOKER;
        case (TypeCell::HILLS):
            return dice == ::runebound::dice::HandDice::FOREST_HILLS ||
                   dice == ::runebound::dice::HandDice::HILLS_PLAIN ||
                   dice == ::runebound::dice::HandDice::JOKER;
        case (TypeCell::PLAIN):
            return dice == ::runebound::dice::HandDice::PLAIN ||
                   dice == ::runebound::dice::HandDice::PLAIN_FOREST ||
                   dice == ::runebound::dice::HandDice::HILLS_PLAIN ||
                   dice == ::runebound::dice::HandDice::JOKER;
        case (TypeCell::TOWN):
            return true;
        default:
            return dice == ::runebound::dice::HandDice::JOKER;
    }
}

void to_json(nlohmann::json &json, const Map &map) {
    json["m_map"] = map.m_map;
    json["m_height"] = map.m_height;
    json["m_width"] = map.m_width;
    json["m_rivers"] = map.m_rivers;
    json["directions_odd_row"] = map.directions_odd_row;
    json["directions_even_row"] = map.directions_even_row;
}

void from_json(const nlohmann::json &json, Map &map) {
    map.m_map = json["m_map"];
}

}  // namespace map
}  // namespace runebound
