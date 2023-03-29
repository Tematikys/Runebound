#include "map.hpp"
#include <algorithm>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
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

void Map::make_rivers() {
    m_rivers.insert({Point(2, 11), Point(3, 11)});
    m_rivers.insert({Point(3, 11), Point(2, 11)});
    m_rivers.insert({Point(3, 10), Point(3, 11)});
    m_rivers.insert({Point(3, 11), Point(3, 10)});
    m_rivers.insert({Point(4, 9), Point(5, 10)});
    m_rivers.insert({Point(5, 10), Point(4, 9)});
    m_rivers.insert({Point(4, 9), Point(5, 9)});
    m_rivers.insert({Point(5, 9), Point(4, 9)});
    m_rivers.insert({Point(5, 8), Point(5, 9)});
    m_rivers.insert({Point(5, 9), Point(5, 8)});
    m_rivers.insert({Point(5, 8), Point(6, 8)});
    m_rivers.insert({Point(6, 8), Point(5, 8)});
    m_rivers.insert({Point(5, 8), Point(5, 7)});
    m_rivers.insert({Point(5, 7), Point(5, 8)});
    m_rivers.insert({Point(5, 7), Point(4, 7)});
    m_rivers.insert({Point(4, 7), Point(5, 7)});
    m_rivers.insert({Point(5, 3), Point(6, 4)});
    m_rivers.insert({Point(6, 4), Point(5, 3)});
    m_rivers.insert({Point(6, 3), Point(6, 4)});
    m_rivers.insert({Point(6, 4), Point(6, 3)});
    m_rivers.insert({Point(6, 3), Point(7, 4)});
    m_rivers.insert({Point(7, 4), Point(6, 3)});
    m_rivers.insert({Point(6, 4), Point(7, 4)});
    m_rivers.insert({Point(7, 4), Point(6, 4)});
    m_rivers.insert({Point(6, 5), Point(7, 4)});
    m_rivers.insert({Point(7, 4), Point(6, 5)});
    m_rivers.insert({Point(7, 3), Point(7, 4)});
    m_rivers.insert({Point(7, 4), Point(7, 3)});
    m_rivers.insert({Point(7, 3), Point(8, 3)});
    m_rivers.insert({Point(8, 3), Point(7, 3)});
    m_rivers.insert({Point(7, 3), Point(8, 4)});
    m_rivers.insert({Point(8, 4), Point(7, 3)});
    m_rivers.insert({Point(8, 2), Point(8, 3)});
    m_rivers.insert({Point(8, 3), Point(8, 2)});
    m_rivers.insert({Point(9, 2), Point(8, 3)});
    m_rivers.insert({Point(8, 3), Point(9, 2)});
    m_rivers.insert({Point(9, 2), Point(9, 3)});
    m_rivers.insert({Point(9, 3), Point(9, 2)});
    m_rivers.insert({Point(9, 1), Point(10, 1)});
    m_rivers.insert({Point(10, 1), Point(9, 1)});
    m_rivers.insert({Point(9, 1), Point(9, 0)});
    m_rivers.insert({Point(9, 0), Point(9, 1)});
    m_rivers.insert({Point(9, 1), Point(10, 0)});
    m_rivers.insert({Point(10, 0), Point(9, 1)});
    m_rivers.insert({Point(8, 1), Point(9, 0)});
    m_rivers.insert({Point(9, 0), Point(8, 1)});
    m_rivers.insert({Point(8, 0), Point(9, 0)});
    m_rivers.insert({Point(9, 0), Point(8, 0)});

    m_rivers.insert({Point(8, 14), Point(9, 14)});
    m_rivers.insert({Point(9, 14), Point(8, 14)});
    m_rivers.insert({Point(8, 13), Point(9, 14)});
    m_rivers.insert({Point(9, 14), Point(8, 13)});
    m_rivers.insert({Point(8, 13), Point(9, 13)});
    m_rivers.insert({Point(9, 13), Point(8, 13)});
    m_rivers.insert({Point(9, 12), Point(9, 13)});
    m_rivers.insert({Point(9, 13), Point(9, 12)});
    m_rivers.insert({Point(10, 12), Point(9, 13)});
    m_rivers.insert({Point(9, 13), Point(10, 12)});
    m_rivers.insert({Point(10, 12), Point(10, 13)});
    m_rivers.insert({Point(10, 13), Point(10, 12)});
    m_rivers.insert({Point(11, 12), Point(10, 13)});
    m_rivers.insert({Point(10, 13), Point(11, 12)});

    m_rivers.insert({Point(12, 14), Point(13, 14)});
    m_rivers.insert({Point(13, 14), Point(12, 14)});
    m_rivers.insert({Point(12, 14), Point(12, 13)});
    m_rivers.insert({Point(12, 13), Point(12, 14)});
    m_rivers.insert({Point(11, 12), Point(12, 12)});
    m_rivers.insert({Point(12, 12), Point(11, 12)});
    m_rivers.insert({Point(11, 11), Point(12, 12)});
    m_rivers.insert({Point(12, 12), Point(11, 11)});
    m_rivers.insert({Point(11, 11), Point(12, 11)});
    m_rivers.insert({Point(12, 11), Point(11, 11)});
    m_rivers.insert({Point(11, 11), Point(12, 10)});
    m_rivers.insert({Point(12, 10), Point(11, 11)});
    m_rivers.insert({Point(11, 10), Point(12, 10)});
    m_rivers.insert({Point(12, 10), Point(11, 10)});
}

void Map::make_tokens() {
    m_map[0][0].make_token(AdventureType::MEETING);
    m_map[0][4].make_token(AdventureType::FIGHT);
    m_map[0][9].make_token(AdventureType::FIGHT);
    m_map[0][12].make_token(AdventureType::MEETING);
    m_map[1][7].make_token(AdventureType::RESEARCH);
    m_map[2][3].make_token(AdventureType::MEETING);
    m_map[3][14].make_token(AdventureType::FIGHT);
    m_map[3][0].make_token(AdventureType::RESEARCH);
    m_map[5][4].make_token(AdventureType::FIGHT);
    m_map[4][9].make_token(AdventureType::RESEARCH);
    m_map[5][12].make_token(AdventureType::MEETING);
    m_map[6][14].make_token(AdventureType::RESEARCH);
    m_map[6][2].make_token(AdventureType::RESEARCH);
    m_map[7][0].make_token(AdventureType::MEETING);
    m_map[10][0].make_token(AdventureType::FIGHT);
    m_map[13][0].make_token(AdventureType::RESEARCH);
    m_map[12][2].make_token(AdventureType::MEETING);
    m_map[12][5].make_token(AdventureType::FIGHT);
    m_map[11][4].make_token(AdventureType::RESEARCH);
    m_map[8][5].make_token(AdventureType::FIGHT);
    m_map[13][14].make_token(AdventureType::FIGHT);
    m_map[10][14].make_token(AdventureType::MEETING);
    m_map[13][8].make_token(AdventureType::MEETING);
    m_map[11][9].make_token(AdventureType::RESEARCH);
    m_map[9][8].make_token(AdventureType::MEETING);
    m_map[7][9].make_token(AdventureType::FIGHT);
    m_map[9][12].make_token(AdventureType::RESEARCH);
}

void Map::make_road() {
    // TODO
}

void Map::make_special_type_cells() {
    m_map[0][1].make_special_type_cell(SpecialTypeCell::FORTRESS);
    m_map[1][5].make_special_type_cell(SpecialTypeCell::SANCTUARY);
    m_map[3][1].make_special_type_cell(SpecialTypeCell::SETTLEMENT);
    m_map[0][14].make_special_type_cell(SpecialTypeCell::SETTLEMENT);
    m_map[2][12].make_special_type_cell(SpecialTypeCell::SANCTUARY);
    m_map[5][1].make_special_type_cell(SpecialTypeCell::FORTRESS);
    m_map[5][8].make_special_type_cell(SpecialTypeCell::SETTLEMENT);
    m_map[6][8].make_special_type_cell(SpecialTypeCell::SETTLEMENT);
    m_map[6][13].make_special_type_cell(SpecialTypeCell::FORTRESS);
    m_map[7][4].make_special_type_cell(SpecialTypeCell::SANCTUARY);
    m_map[8][13].make_special_type_cell(SpecialTypeCell::SANCTUARY);
    m_map[12][0].make_special_type_cell(SpecialTypeCell::SETTLEMENT);
    m_map[13][6].make_special_type_cell(SpecialTypeCell::SANCTUARY);
    m_map[11][6].make_special_type_cell(SpecialTypeCell::FORTRESS);
    m_map[8][7].make_special_type_cell(SpecialTypeCell::SETTLEMENT);
    m_map[13][12].make_special_type_cell(SpecialTypeCell::FORTRESS);
}

void Map::make_map() {
    m_map.resize(m_size, std::vector<MapCell>(m_size));
    make_cells();
    make_rivers();
    make_tokens();
    make_road();
    make_special_type_cells();
}

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
    json["directions_odd_row"] = map.directions_odd_column;
    json["directions_even_row"] = map.directions_odd_column;
}

void from_json(const nlohmann::json &json, Map &map) {
    map.m_map = json["m_map"];
}

}  // namespace map
}  // namespace runebound
