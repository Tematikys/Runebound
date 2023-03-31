#ifndef MAP_CLIENT_HPP_
#define MAP_CLIENT_HPP_
#include <nlohmann/json.hpp>
#include "point.hpp"
#include <set>
#include "map_cell.hpp"
#include "map.hpp"

namespace runebound::map {
struct MapClient {
private:
    const int m_size;
    const std::set<std::pair<Point, Point>> m_rivers;
    std::vector<std::vector<MapCell>> m_map;
public:
    MapClient() : m_size(STANDARD_SIZE), m_rivers(::runebound::map::make_rivers()), m_map(make_map()) {}

    void reverse_token(int row, int column) {
        m_map[row][column].reverse_token();
    }
};

} // namespace runebound::map
#endif // MAP_CLIENT_HPP_