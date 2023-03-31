#ifndef MAP_CLIENT_HPP_
#define MAP_CLIENT_HPP_
#include <nlohmann/json.hpp>
#include <set>
#include "map.hpp"
#include "map_cell.hpp"
#include "point.hpp"

namespace runebound::map {
struct MapClient {
private:
    friend struct ::runebound::graphics::Board;
    static const int m_size = STANDARD_SIZE;
    const std::set<std::pair<Point, Point>> m_rivers = make_rivers();
    std::vector<std::vector<MapCell>> m_map;

public:
    MapClient() : m_rivers(make_rivers()), m_map(make_map()) {
    }

    void reverse_token(int row, int column) {
        m_map[row][column].reverse_token();
    }

    friend void to_json(nlohmann::json &json, const MapClient &map) {
        json["m_size"] = m_size;
        json["m_rivers"] = map.m_rivers;
        json["m_map"] = map.m_map;
    }

    friend void from_json(const nlohmann::json &json, MapClient &map) {
        map.m_map = json["m_map"];
    }
};

}  // namespace runebound::map
#endif  // MAP_CLIENT_HPP_