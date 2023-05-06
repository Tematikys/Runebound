#include "map_client.hpp"
#include <vector>

namespace runebound::map {

bool MapClient::check_neighbour_in_direction(
    const Point &cell,
    const Point &direction
) const {
    return (cell.x + direction.x >= 0) && (cell.x + direction.x < m_size) &&
           (cell.y + direction.y >= 0) && (cell.y + direction.y < m_size);
}

std::vector<Point> MapClient::get_all_neighbours(const Point &cell) const {
    std::vector<Point> directions;
    if (cell.y % 2 == 0) {
        directions = directions_even_column;
    } else {
        directions = directions_odd_column;
    }
    std::vector<Point> neighbours;
    for (auto direction : directions) {
        if (check_neighbour_in_direction(cell, direction)) {
            neighbours.emplace_back(
                Point(cell.x + direction.x, cell.y + direction.y)
            );
        }
    }
    return neighbours;
}

void to_json(nlohmann::json &json, const MapClient &map) {
    json["m_size"] = map.m_size;
    json["m_rivers"] = map.m_rivers;
    json["m_map"] = map.m_map;
    json["m_territory_name"] = map.m_territory_name;
}

void from_json(const nlohmann::json &json, MapClient &map) {
    map.m_size = json["m_size"];
    map.m_rivers = json["m_rivers"];
    map.m_map = json["m_map"];
    map.m_territory_name = json["m_territory_name"];
}

}  // namespace runebound::map