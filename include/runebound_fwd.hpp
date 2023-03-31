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
}

namespace map {
struct MapCell;
struct Map;
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
namespace map {
std::set<std::pair<Point, Point>> make_rivers();
std::vector<std::vector<MapCell>> make_map();
}  // namespace map
}  // namespace runebound

#endif  // RUNEBOUND_FWD_HPP_
