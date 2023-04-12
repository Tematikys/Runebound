#include <set>
#include <vector>
#include "map.hpp"
#include "map_cell.hpp"
#include "point.hpp"
#include "runebound_fwd.hpp"

namespace runebound::map {
std::set<std::pair<Point, Point>> make_rivers() {
    std::set<std::pair<Point, Point>> rivers;
    rivers.insert({Point(2, 11), Point(3, 11)});
    rivers.insert({Point(3, 11), Point(2, 11)});
    rivers.insert({Point(3, 10), Point(3, 11)});
    rivers.insert({Point(3, 11), Point(3, 10)});
    rivers.insert({Point(4, 9), Point(5, 10)});
    rivers.insert({Point(5, 10), Point(4, 9)});
    rivers.insert({Point(4, 9), Point(5, 9)});
    rivers.insert({Point(5, 9), Point(4, 9)});
    rivers.insert({Point(5, 8), Point(5, 9)});
    rivers.insert({Point(5, 9), Point(5, 8)});
    rivers.insert({Point(5, 8), Point(6, 8)});
    rivers.insert({Point(6, 8), Point(5, 8)});
    rivers.insert({Point(5, 8), Point(5, 7)});
    rivers.insert({Point(5, 7), Point(5, 8)});
    rivers.insert({Point(5, 7), Point(4, 7)});
    rivers.insert({Point(4, 7), Point(5, 7)});
    rivers.insert({Point(5, 3), Point(6, 4)});
    rivers.insert({Point(6, 4), Point(5, 3)});
    rivers.insert({Point(6, 3), Point(6, 4)});
    rivers.insert({Point(6, 4), Point(6, 3)});
    rivers.insert({Point(6, 3), Point(7, 4)});
    rivers.insert({Point(7, 4), Point(6, 3)});
    rivers.insert({Point(6, 4), Point(7, 4)});
    rivers.insert({Point(7, 4), Point(6, 4)});
    rivers.insert({Point(6, 5), Point(7, 4)});
    rivers.insert({Point(7, 4), Point(6, 5)});
    rivers.insert({Point(7, 3), Point(7, 4)});
    rivers.insert({Point(7, 4), Point(7, 3)});
    rivers.insert({Point(7, 3), Point(8, 3)});
    rivers.insert({Point(8, 3), Point(7, 3)});
    rivers.insert({Point(7, 3), Point(8, 4)});
    rivers.insert({Point(8, 4), Point(7, 3)});
    rivers.insert({Point(8, 2), Point(8, 3)});
    rivers.insert({Point(8, 3), Point(8, 2)});
    rivers.insert({Point(9, 2), Point(8, 3)});
    rivers.insert({Point(8, 3), Point(9, 2)});
    rivers.insert({Point(9, 2), Point(9, 3)});
    rivers.insert({Point(9, 3), Point(9, 2)});
    rivers.insert({Point(9, 1), Point(10, 1)});
    rivers.insert({Point(10, 1), Point(9, 1)});
    rivers.insert({Point(9, 1), Point(9, 0)});
    rivers.insert({Point(9, 0), Point(9, 1)});
    rivers.insert({Point(9, 1), Point(10, 0)});
    rivers.insert({Point(10, 0), Point(9, 1)});
    rivers.insert({Point(8, 1), Point(9, 0)});
    rivers.insert({Point(9, 0), Point(8, 1)});
    rivers.insert({Point(8, 0), Point(9, 0)});
    rivers.insert({Point(9, 0), Point(8, 0)});
    rivers.insert({Point(8, 14), Point(9, 14)});
    rivers.insert({Point(9, 14), Point(8, 14)});
    rivers.insert({Point(8, 13), Point(9, 14)});
    rivers.insert({Point(9, 14), Point(8, 13)});
    rivers.insert({Point(8, 13), Point(9, 13)});
    rivers.insert({Point(9, 13), Point(8, 13)});
    rivers.insert({Point(9, 12), Point(9, 13)});
    rivers.insert({Point(9, 13), Point(9, 12)});
    rivers.insert({Point(10, 12), Point(9, 13)});
    rivers.insert({Point(9, 13), Point(10, 12)});
    rivers.insert({Point(10, 12), Point(10, 13)});
    rivers.insert({Point(10, 13), Point(10, 12)});
    rivers.insert({Point(11, 12), Point(10, 13)});
    rivers.insert({Point(10, 13), Point(11, 12)});
    rivers.insert({Point(12, 14), Point(13, 14)});
    rivers.insert({Point(13, 14), Point(12, 14)});
    rivers.insert({Point(12, 14), Point(12, 13)});
    rivers.insert({Point(12, 13), Point(12, 14)});
    rivers.insert({Point(11, 12), Point(12, 12)});
    rivers.insert({Point(12, 12), Point(11, 12)});
    rivers.insert({Point(11, 11), Point(12, 12)});
    rivers.insert({Point(12, 12), Point(11, 11)});
    rivers.insert({Point(11, 11), Point(12, 11)});
    rivers.insert({Point(12, 11), Point(11, 11)});
    rivers.insert({Point(11, 11), Point(12, 10)});
    rivers.insert({Point(12, 10), Point(11, 11)});
    rivers.insert({Point(11, 10), Point(12, 10)});
    rivers.insert({Point(12, 10), Point(11, 10)});
    return rivers;
}

namespace {
void make_row(
    std::vector<std::vector<MapCell>> &map,
    int row,
    const std::vector<std::pair<TypeCell, int>> &elements
) {
    int column = 0;
    for (const auto &one_type : elements) {
        for (int i = 0; i < one_type.second; ++i) {
            map[row][column] = MapCell(one_type.first);
            column += 1;
        }
    }
}

void make_cells(std::vector<std::vector<MapCell>> &map) {
    make_row(
        map, 0,
        std::vector{
            std::pair(TypeCell::PLAIN, 7), std::pair(TypeCell::FOREST, 4),
            std::pair(TypeCell::PLAIN, 2), std::pair(TypeCell::HILLS, 1),
            std::pair(TypeCell::MOUNTAINS, 1)}
    );
    make_row(
        map, 1,
        std::vector{
            std::pair(TypeCell::FOREST, 2), std::pair(TypeCell::PLAIN, 5),
            std::pair(TypeCell::FOREST, 4), std::pair(TypeCell::TOWN, 1),
            std::pair(TypeCell::HILLS, 1), std::pair(TypeCell::MOUNTAINS, 2)}
    );
    make_row(
        map, 2,
        std::vector{
            std::pair(TypeCell::FOREST, 2), std::pair(TypeCell::MOUNTAINS, 1),
            std::pair(TypeCell::PLAIN, 5), std::pair(TypeCell::FOREST, 1),
            std::pair(TypeCell::PLAIN, 3), std::pair(TypeCell::MOUNTAINS, 3)}
    );
    make_row(
        map, 3,
        std::vector{
            std::pair(TypeCell::FOREST, 1), std::pair(TypeCell::PLAIN, 2),
            std::pair(TypeCell::HILLS, 1), std::pair(TypeCell::PLAIN, 8),
            std::pair(TypeCell::MOUNTAINS, 3)}
    );
    make_row(
        map, 4,
        std::vector{
            std::pair(TypeCell::PLAIN, 5), std::pair(TypeCell::WATER, 1),
            std::pair(TypeCell::PLAIN, 2), std::pair(TypeCell::FOREST, 1),
            std::pair(TypeCell::PLAIN, 1), std::pair(TypeCell::WATER, 1),
            std::pair(TypeCell::PLAIN, 2), std::pair(TypeCell::HILLS, 2)}
    );
    make_row(
        map, 5,
        std::vector{
            std::pair(TypeCell::PLAIN, 1), std::pair(TypeCell::HILLS, 1),
            std::pair(TypeCell::PLAIN, 2), std::pair(TypeCell::WATER, 2),
            std::pair(TypeCell::TOWN, 1), std::pair(TypeCell::PLAIN, 4),
            std::pair(TypeCell::FOREST, 2), std::pair(TypeCell::HILLS, 2)}
    );
    make_row(
        map, 6,
        std::vector{
            std::pair(TypeCell::HILLS, 1), std::pair(TypeCell::PLAIN, 1),
            std::pair(TypeCell::FOREST, 2), std::pair(TypeCell::PLAIN, 6),
            std::pair(TypeCell::FOREST, 3), std::pair(TypeCell::HILLS, 2)}
    );
    make_row(
        map, 7,
        std::vector{
            std::pair(TypeCell::HILLS, 1), std::pair(TypeCell::PLAIN, 4),
            std::pair(TypeCell::WATER, 1), std::pair(TypeCell::PLAIN, 3),
            std::pair(TypeCell::MOUNTAINS, 1), std::pair(TypeCell::PLAIN, 2),
            std::pair(TypeCell::HILLS, 1), std::pair(TypeCell::PLAIN, 2)}
    );
    make_row(
        map, 8,
        std::vector{
            std::pair(TypeCell::PLAIN, 10), std::pair(TypeCell::MOUNTAINS, 1),
            std::pair(TypeCell::PLAIN, 4)}
    );
    make_row(
        map, 9,
        std::vector{
            std::pair(TypeCell::FOREST, 1), std::pair(TypeCell::PLAIN, 10),
            std::pair(TypeCell::WATER, 1), std::pair(TypeCell::PLAIN, 3)}
    );
    make_row(
        map, 10,
        std::vector{
            std::pair(TypeCell::FOREST, 1), std::pair(TypeCell::PLAIN, 1),
            std::pair(TypeCell::TOWN, 1), std::pair(TypeCell::HILLS, 1),
            std::pair(TypeCell::PLAIN, 3), std::pair(TypeCell::HILLS, 1),
            std::pair(TypeCell::PLAIN, 6), std::pair(TypeCell::FOREST, 1)}
    );
    make_row(
        map, 11,
        std::vector{
            std::pair(TypeCell::PLAIN, 4), std::pair(TypeCell::HILLS, 1),
            std::pair(TypeCell::MOUNTAINS, 1), std::pair(TypeCell::PLAIN, 1),
            std::pair(TypeCell::HILLS, 1), std::pair(TypeCell::WATER, 2),
            std::pair(TypeCell::PLAIN, 3), std::pair(TypeCell::TOWN, 1),
            std::pair(TypeCell::FOREST, 1)}
    );
    make_row(
        map, 12,
        std::vector{
            std::pair(TypeCell::PLAIN, 3), std::pair(TypeCell::HILLS, 2),
            std::pair(TypeCell::MOUNTAINS, 2), std::pair(TypeCell::HILLS, 3),
            std::pair(TypeCell::PLAIN, 5)}
    );
    make_row(
        map, 13,
        std::vector{
            std::pair(TypeCell::PLAIN, 3), std::pair(TypeCell::HILLS, 2),
            std::pair(TypeCell::MOUNTAINS, 3), std::pair(TypeCell::HILLS, 1),
            std::pair(TypeCell::MOUNTAINS, 1), std::pair(TypeCell::PLAIN, 5)}
    );
    make_row(
        map, 14,
        std::vector{
            std::pair(TypeCell::PLAIN, 3), std::pair(TypeCell::HILLS, 2),
            std::pair(TypeCell::MOUNTAINS, 3), std::pair(TypeCell::HILLS, 1),
            std::pair(TypeCell::MOUNTAINS, 1), std::pair(TypeCell::PLAIN, 5)}
    );
}

void make_tokens(std::vector<std::vector<MapCell>> &map) {
    map[0][0].make_token(AdventureType::MEETING);
    map[0][4].make_token(AdventureType::FIGHT);
    map[0][9].make_token(AdventureType::FIGHT);
    map[0][12].make_token(AdventureType::MEETING);
    map[1][7].make_token(AdventureType::RESEARCH);
    map[2][3].make_token(AdventureType::MEETING);
    map[3][14].make_token(AdventureType::FIGHT);
    map[3][0].make_token(AdventureType::RESEARCH);
    map[5][4].make_token(AdventureType::FIGHT);
    map[4][9].make_token(AdventureType::RESEARCH);
    map[5][12].make_token(AdventureType::MEETING);
    map[6][14].make_token(AdventureType::RESEARCH);
    map[6][2].make_token(AdventureType::RESEARCH);
    map[7][0].make_token(AdventureType::MEETING);
    map[10][0].make_token(AdventureType::FIGHT);
    map[13][0].make_token(AdventureType::RESEARCH);
    map[12][2].make_token(AdventureType::MEETING);
    map[12][5].make_token(AdventureType::FIGHT);
    map[11][4].make_token(AdventureType::RESEARCH);
    map[8][5].make_token(AdventureType::FIGHT);
    map[13][14].make_token(AdventureType::FIGHT);
    map[10][14].make_token(AdventureType::MEETING);
    map[13][8].make_token(AdventureType::MEETING);
    map[11][9].make_token(AdventureType::RESEARCH);
    map[9][8].make_token(AdventureType::MEETING);
    map[7][9].make_token(AdventureType::FIGHT);
    map[9][12].make_token(AdventureType::RESEARCH);
}

void make_all_roads_in_row(
    std::vector<std::vector<MapCell>> &map,
    int row,
    const std::vector<int> columns
) {
    for (const auto &column : columns) {
        map[row][column].make_road();
    }
}

void make_road(std::vector<std::vector<MapCell>> &map) {
    make_all_roads_in_row(map, 0, {5});
    make_all_roads_in_row(map, 1, {5});
    make_all_roads_in_row(map, 2, {6, 7, 9, 10});
    make_all_roads_in_row(map, 3, {6, 8});
    make_all_roads_in_row(map, 4, {6});
    make_all_roads_in_row(map, 6, {6});
    make_all_roads_in_row(map, 7, {6, 7});
    make_all_roads_in_row(map, 8, {7});
    make_all_roads_in_row(map, 9, {6, 5, 4, 3, 7});
    make_all_roads_in_row(map, 10, {8, 9, 11});
    make_all_roads_in_row(map, 11, {10, 12});
    make_all_roads_in_row(map, 12, {10});
}

void make_special_type_cells(std::vector<std::vector<MapCell>> &map) {
    map[0][1].make_special_type_cell(SpecialTypeCell::FORTRESS);
    map[1][5].make_special_type_cell(SpecialTypeCell::SANCTUARY);
    map[3][1].make_special_type_cell(SpecialTypeCell::SETTLEMENT);
    map[0][14].make_special_type_cell(SpecialTypeCell::SETTLEMENT);
    map[2][12].make_special_type_cell(SpecialTypeCell::SANCTUARY);
    map[5][1].make_special_type_cell(SpecialTypeCell::FORTRESS);
    map[5][8].make_special_type_cell(SpecialTypeCell::SETTLEMENT);
    map[6][8].make_special_type_cell(SpecialTypeCell::SETTLEMENT);
    map[6][13].make_special_type_cell(SpecialTypeCell::FORTRESS);
    map[7][4].make_special_type_cell(SpecialTypeCell::SANCTUARY);
    map[8][13].make_special_type_cell(SpecialTypeCell::SANCTUARY);
    map[12][0].make_special_type_cell(SpecialTypeCell::SETTLEMENT);
    map[13][6].make_special_type_cell(SpecialTypeCell::SANCTUARY);
    map[11][6].make_special_type_cell(SpecialTypeCell::FORTRESS);
    map[8][7].make_special_type_cell(SpecialTypeCell::SETTLEMENT);
    map[13][12].make_special_type_cell(SpecialTypeCell::FORTRESS);
}

}  // namespace

std::vector<std::vector<MapCell>> make_map() {
    std::vector<std::vector<MapCell>> map(
        STANDARD_SIZE, std::vector<MapCell>(STANDARD_SIZE)
    );
    make_cells(map);
    make_tokens(map);
    make_road(map);
    make_special_type_cells(map);
    return map;
}

std::map<std::string, std::vector<Point>> make_territory_name() {
    std::map<std::string, std::vector<Point>> territory_name;
    territory_name["Starfall Forest"] = {Point(2, 0), Point(1, 1)};
    territory_name["Rogue Peak"] = {Point(2, 2)};
    territory_name["Everton"] = {Point(3, 1)};
    territory_name["Caer Loek"] = {Point(0, 1)};
    territory_name["Suvar'start Haven"] = {Point(1, 5)};
    territory_name["Cursed ruins"] = {Point(3, 3)};
    territory_name["Telsvanian Plains"] = {
        Point(1, 3), Point(2, 4), Point(2, 5)};
    territory_name["The Quagmire of the Black Wing"] = {
        Point(5, 4), Point(5, 5), Point(4, 5)};
    territory_name["Telis Castle"] = {Point(5, 1)};
    territory_name["Talamir"] = {Point(5, 6)};
    territory_name["Robber freemen"] = {Point(1, 8)};
    territory_name["The Thicket of Despondency"] = {
        Point(0, 7), Point(0, 8), Point(0, 9)};
    territory_name["Valley of Souls"] = {Point(0, 13), Point(1, 12)};
    territory_name["Underground City"] = {Point(0, 14)};
    territory_name["Snake Forest"] = {Point(4, 8)};
    territory_name["Echo Lake"] = {Point(4, 10)};
    territory_name["Smithy"] = {Point(1, 11)};
    territory_name["Itraitova Forge"] = {Point(2, 12)};
    territory_name["Sharp cliffs"] = {Point(3, 14), Point(3, 13), Point(3, 12)};
    territory_name["Ash Hills"] = {
        Point(6, 14), Point(5, 14), Point(5, 13), Point(4, 12)};
    territory_name["Hedge"] = {Point(6, 2), Point(6, 3)};
    territory_name["Howling Dol"] = {Point(6, 0), Point(7, 0)};
    territory_name["Hunting Circle"] = {Point(7, 4)};
    territory_name["Lake Kernan"] = {Point(7, 5)};
    territory_name["Kellos' Breath"] = {Point(8, 0), Point(7, 1), Point(8, 2)};
    territory_name["Stub"] = {Point(9, 0), Point(10, 0)};
    territory_name["Downsmoor"] = {Point(10, 2)};
    territory_name["Bright dol"] = {Point(12, 0)};
    territory_name["Misty Plains"] = {
        Point(11, 1), Point(11, 2), Point(12, 2), Point(11, 3)};
    territory_name["Lowlands of Bitterness"] = {Point(12, 3), Point(12, 4)};
    territory_name["Mountains of Despair"] = {Point(12, 6), Point(11, 5)};
    territory_name["Madness Pass"] = {Point(13, 6)};
    territory_name["The firmament"] = {Point(11, 6)};
    territory_name["Blue Haze Hills"] = {
        Point(11, 7), Point(12, 8), Point(12, 9)};
    territory_name["Moonlight Swamp"] = {Point(11, 8), Point(11, 9)};
    territory_name["Zedrik Crossing"] = {Point(11, 10)};
    territory_name["Southern Plains"] = {
        Point(13, 10), Point(12, 11), Point(12, 12)};
    territory_name["Sandergard"] = {Point(13, 12)};
    territory_name["River Patrol"] = {Point(11, 13)};
    territory_name["Green Forest"] = {Point(11, 14), Point(10, 14)};
    territory_name["Aris Sanctuary"] = {Point(8, 13)};
    territory_name["Lake of Tears"] = {Point(9, 11)};
    territory_name["The Fangs of the Mennara"] = {Point(7, 9), Point(8, 10)};
    territory_name["Fort Rodrik"] = {Point(6, 13)};
    territory_name["Trading Hail"] = {Point(8, 7)};
    territory_name["River lands"] = {Point(6, 7), Point(7, 6), Point(8, 6)};
    return territory_name;
}

void make_connections_between_territory_names_and_cells(
    std::vector<std::vector<MapCell>> &map,
    const std::map<std::string, std::vector<Point>> &territory_name
) {
    for (const auto &territory : territory_name) {
        for (const auto &cell : territory.second) {
            map[cell.x][cell.y].make_name_territory(territory.first);
        }
    }
}
}  // namespace runebound::map
