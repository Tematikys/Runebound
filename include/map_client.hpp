#ifndef MAP_CLIENT_HPP_
#define MAP_CLIENT_HPP_

#include <map>
#include <nlohmann/json.hpp>
#include <set>
#include <string>
#include "map.hpp"
#include "map_cell.hpp"
#include "point.hpp"
#include "runebound_fwd.hpp"
#include "tokens.hpp"

namespace runebound::map {
void to_json(nlohmann::json &json, const MapClient &map);
void from_json(const nlohmann::json &json, MapClient &map);

struct MapClient {
private:
    friend struct ::runebound::graphics::Board;
    const std::vector<Point> directions_odd_column{{-1, 0}, {0, 1},  {1, 1},
                                                   {1, 0},  {1, -1}, {0, -1}};
    const std::vector<Point> directions_even_column{{-1, 0}, {-1, 1}, {0, 1},
                                                    {1, 0},  {0, -1}, {-1, -1}};
    const std::map<std::string, std::vector<Point>> m_territory_name;
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
    MapClient()
        : m_rivers(make_rivers()),
          m_map(make_map()),
          m_territory_name(make_territory_name()) {
        make_connections_between_territory_names_and_cells(
            m_map, m_territory_name
        );
    }

    explicit MapClient(const Map &map) {
        std::vector<std::vector<MapCell>> m_map = map.get_full_map();
    }

    MapClient(const MapClient &) = default;
    MapClient(MapClient &&) = default;

    MapClient &operator=(const MapClient &other) {
        m_map = other.m_map;
        return *this;
    }

    MapClient &operator=(MapClient &&other) {
        m_map = other.m_map;
        return *this;
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