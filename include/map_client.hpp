#ifndef MAP_CLIENT_HPP_
#define MAP_CLIENT_HPP_

#include <fstream>
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
    friend struct ::runebound::graphics::Client;
    const std::vector<Point> directions_odd_column{{-1, 0}, {0, 1},  {1, 1},
                                                   {1, 0},  {1, -1}, {0, -1}};
    const std::vector<Point> directions_even_column{{-1, 0}, {-1, 1}, {0, 1},
                                                    {1, 0},  {0, -1}, {-1, -1}};
    std::map<std::string, std::vector<Point>> m_territory_name;
    int m_size = STANDARD_SIZE;
    std::set<std::pair<Point, Point>> m_rivers;
    std::vector<std::vector<MapCell>> m_map;

    [[nodiscard]] bool check_neighbour_in_direction(
        const Point &cell,
        const Point &direction
    ) const;

    [[nodiscard]] std::vector<Point> get_all_neighbours(const Point &cell
    ) const;

public:
    MapClient() {
        nlohmann::json json;
        std::ifstream in("data/json/map/map.json");
        in >> json;
        ::runebound::map::from_json(json, *this);
    }

    explicit MapClient(const Map &map) {
        m_map = map.m_map;
        m_size = map.m_size;
        m_territory_name = map.m_territory_name;
        m_rivers = map.m_rivers;
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

    const std::map<std::string, std::vector<Point>> &get_territory_name() const;
    int get_size() const;
    const std::set<std::pair<Point, Point>> &get_rivers() const;
    const std::vector<std::vector<MapCell>> &get_map() const;

    friend void to_json(nlohmann::json &json, const MapClient &map);

    friend void from_json(const nlohmann::json &json, MapClient &map);
};

}  // namespace runebound::map
#endif  // MAP_CLIENT_HPP_