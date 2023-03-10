#include "map_cell.hpp"
#include <cassert>
#include <memory>
#include <vector>
#include "nlohmann/json.hpp"

namespace runebound {
namespace map {

nlohmann::json serialize_map_cell(const MapCell& map_cell) {
    nlohmann::json json_map_cell;
    json_map_cell["m_type_cell"] = map_cell.m_type_cell;
    json_map_cell["m_token"] = map_cell.m_token;
    json_map_cell["m_side_token"] = map_cell.m_side_token;
    return json_map_cell;
}

MapCell deserialize_map_cell(const nlohmann::json& json) {
    MapCell result;
    result.m_type_cell = json["m_type_cell"];
    result.m_token = json["m_token"];
    result.m_side_token = json["m_side_token"];
    return result;
}
} // namespace map
} // namespace runebound