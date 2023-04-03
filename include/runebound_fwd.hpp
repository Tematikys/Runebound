#ifndef RUNEBOUND_FWD_HPP_
#define RUNEBOUND_FWD_HPP_

#include <chrono>
#include <random>
#include <set>

namespace runebound {
enum class AdventureType { MEETING, RESEARCH, FIGHT, NOTHING };

enum class Side { FRONT, BACK };

static std::mt19937 rng(
    std::chrono::steady_clock::now().time_since_epoch().count()
);

struct Point;

namespace token {
struct Token;
}  // namespace token

namespace character {
struct Character;
struct CharacterClient;
}

namespace map {
struct MapCell;
struct Map;
struct MapClient;
}  // namespace map

namespace game {
struct Game;
}

namespace cards {
struct CardAdventure;
struct CardResearch;
}  // namespace cards

namespace client {
struct Client;
}

namespace graphics {
struct Board;
}

namespace map {
std::set<std::pair<Point, Point>> make_rivers();
std::vector<std::vector<MapCell>> make_map();
std::map<std::string, std::vector<Point>> make_territory_name();
void make_connections_between_territory_names_and_cells(
    std::vector<std::vector<MapCell>> &map,
    const std::map<std::string, std::vector<Point>> &territory_name
);
}  // namespace map
}  // namespace runebound

#endif  // RUNEBOUND_FWD_HPP_
