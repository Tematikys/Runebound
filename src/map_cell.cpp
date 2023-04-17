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
    json["m_special_type_cell"] = map_cell.m_special_type_cell;
    json["m_road"] = map_cell.m_road;
    json["m_territory_name"] = map_cell.m_territory_name;
}

void from_json(const nlohmann::json &json, MapCell &map_cell) {
    map_cell.m_type_cell = json["m_type_cell"];
    map_cell.m_token = json["m_token"];
    map_cell.m_side_token = json["m_side_token"];
    map_cell.m_special_type_cell  = json["m_special_type_cell"];
    map_cell.m_road = json["m_road"];
    map_cell.m_territory_name = json["m_territory_name"];
}
}  // namespace map
}  // namespace runebound