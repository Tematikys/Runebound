#include "map_cell.hpp"
#include <cassert>
#include <memory>
#include <nlohmann/json.hpp>
#include <vector>

namespace runebound {
namespace map {

void to_json(nlohmann::json &json, const MapCell &map_cell) {
    json["m_type_cell"] = map_cell.m_type_cell;
    json["m_token"] = map_cell.m_token;
    json["m_side_token"] = map_cell.m_side_token;
}

void from_json(const nlohmann::json &json, MapCell &map_cell) {
    map_cell.m_type_cell = json["m_type_cell"];
    map_cell.m_token = json["m_token"];
    map_cell.m_side_token = json["m_side_token"];
}
}  // namespace map
}  // namespace runebound