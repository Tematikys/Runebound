#ifndef RUNEBOUND_FWD_HPP_
#define RUNEBOUND_FWD_HPP_

#include <chrono>
#include <map>
#include <random>
#include <set>

namespace runebound {
enum class AdventureType { MEETING, RESEARCH, FIGHT, NOTHING };

enum class Side { FRONT, BACK };

static std::mt19937 rng(
    std::chrono::steady_clock::now().time_since_epoch().count()
);

enum class Characteristic { BODY, INTELLIGENCE, SPIRIT };

struct Point;

namespace character {
struct Character;
struct CharacterClient;
}  // namespace character

namespace trade {
struct Product;
struct Shop;
}  // namespace trade

namespace fight {
struct FightToken;
struct Fight;
struct TokenHandCount;
struct Enemy;
struct FightClient;
}  // namespace fight

namespace generator {
void generate_characters();
void generate_cards_fight();
void generate_cards_meeting();
void generate_cards_research();
void generate_products();
void generate_map();
}  // namespace generator

namespace map {
struct MapCell;
struct Map;
struct MapClient;
}  // namespace map

namespace game {
struct Game;
struct GameClient;
}  // namespace game

namespace cards {
struct Meeting;
struct CardAdventure;
struct CardResearch;
struct CardFight;
struct SkillCard;
struct CardMeeting;
}  // namespace cards

namespace client {
struct Client;
}

namespace graphics {
struct Board;
}

}  // namespace runebound

#endif  // RUNEBOUND_FWD_HPP_
