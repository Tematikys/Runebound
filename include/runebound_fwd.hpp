#ifndef RUNEBOUND_FWD_HPP_
#define RUNEBOUND_FWD_HPP_

namespace runebound {
enum class AdventureType { MEETING, RESEARCH, FIGHT, NOTHING };

enum class Side { FRONT, BACK };
namespace token {
struct Token;
} // namespace token

namespace map {
struct MapCell;
} // namespace token

namespace cards {
struct CardAdventure;
struct CardResearch;
} // namespace cards

}  // namespace runebound

#endif  // RUNEBOUND_FWD_HPP_
