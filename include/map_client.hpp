#ifndef MAP_CLIENT_HPP_
#define MAP_CLIENT_HPP_
#include <nlohmann/json.hpp>
#include <set>
#include "map.hpp"
#include "map_cell.hpp"
#include "point.hpp"
#include "runebound_fwd.hpp"

namespace runebound::map {
void to_json(nlohmann::json &json, const MapClient &map);

struct MapClient {
private:
    friend struct ::runebound::graphics::Board;
    const std::vector<Point> directions_odd_column{{-1, 0}, {0, 1},  {1, 1},
                                                   {1, 0},  {1, -1}, {0, -1}};
    const std::vector<Point> directions_even_column{{-1, 0}, {-1, 1}, {0, 1},
                                                    {1, 0},  {0, -1}, {-1, -1}};
    const int m_size = STANDARD_SIZE;
    const std::set<std::pair<Point, Point>> m_rivers = make_rivers();
    std::vector<std::vector<MapCell>> m_map;

    [[nodiscard]] bool check_neighbour_in_direction(
        const Point &cell,
        const Point &direction
    ) const;

    [[nodiscard]] std::vector<Point> get_all_neighbours(const Point &cell
    ) const;

public:
    MapClient() : m_rivers(make_rivers()), m_map(make_map()) {
    }

    void reverse_token(int row, int column) {
        m_map[row][column].reverse_token();
    }

    friend void to_json(nlohmann::json &json, const MapClient &map);

    friend void from_json(const nlohmann::json &json, MapClient &map) {
        map.m_map = json["m_map"];
    }
};

}  // namespace runebound::map
#endif  // MAP_CLIENT_HPP_