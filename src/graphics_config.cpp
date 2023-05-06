#include <graphics_config.hpp>

namespace runebound::graphics {
const ::std::vector<::std::pair<::std::string, ::std::string>> FONTS = {
    {"data/fonts/FreeMono.ttf", "FreeMono"}};
const ::std::vector<::std::pair<::std::string, ::std::string>> IMAGES = {
    {"data/images/LISSA.png", "Lissa"},
    {"data/images/CORBIN.png", "Corbin"},
    {"data/images/ELDER_MOK.png", "Elder Mok"},
    {"data/images/LAUREL_FROM_BLOODWOOD.png", "Laurel from Bloodwood"},
    {"data/images/LORD_HAWTHORNE.png", "Lord Hawthorne"},
    {"data/images/MASTER_THORN.png", "Master Thorn"}};
const int HEXAGON_RADIUS = 26;
const SDL_Color SELECTED_COLOR = {0xFF, 0xF7, 0x00, 0xFF};
const ::std::map<::runebound::map::TypeCell, SDL_Color> CELL_FILL_COLOR = {
    {::runebound::map::TypeCell::WATER, {0x37, 0x1A, 0xCA, 0xFF}},
    {::runebound::map::TypeCell::FOREST, {0x15, 0x66, 0x1D, 0xFF}},
    {::runebound::map::TypeCell::MOUNTAINS, {0x68, 0x7C, 0x7C, 0xFF}},
    {::runebound::map::TypeCell::HILLS, {0x72, 0xB0, 0x34, 0xFF}},
    {::runebound::map::TypeCell::PLAIN, {0x11, 0xF0, 0x4D, 0xFF}},
    {::runebound::map::TypeCell::TOWN, {0x03, 0x07, 0x06, 0xFF}}};
const ::std::map<::runebound::map::SpecialTypeCell, SDL_Color> SPECIAL_COLOR = {
    {::runebound::map::SpecialTypeCell::SETTLEMENT, {0xE2, 0xE2, 0x18, 0xFF}},
    {::runebound::map::SpecialTypeCell::SANCTUARY, {0xFF, 0xFF, 0xFF, 0xFF}},
    {::runebound::map::SpecialTypeCell::FORTRESS, {0xA0, 0xA0, 0xA0, 0xFF}}};
const ::std::map<::runebound::AdventureType, SDL_Color> ADVENTURE_COLOR = {
    {::runebound::AdventureType::MEETING, {0x9D, 0x00, 0xC4, 0xFF}},
    {::runebound::AdventureType::RESEARCH, {0x00, 0x9F, 0x00, 0xFF}},
    {::runebound::AdventureType::FIGHT, {0xC4, 0x90, 0x00, 0xFF}}};
const ::std::map<::std::pair<int, int>, ::std::pair<int, int>>
    RIVER_DIRECTIONS = {{{-1, 0}, {3, 4}}, {{-1, -1}, {2, 3}},
                        {{-1, 1}, {4, 5}}, {{1, 0}, {0, 1}},
                        {{1, -1}, {1, 2}}, {{1, 1}, {5, 0}}};
const ::std::string WINDOW_TITLE = "Runebound-0.2";
const int WINDOW_X_OFFSET = 25;
const int WINDOW_Y_OFFSET = 25;
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int WINDOW_FPS = 60;
const ::std::map<::runebound::dice::HandDice, ::std::pair<SDL_Color, SDL_Color>>
    DICE_COLOR = {
        {::runebound::dice::HandDice::PLAIN,
         {{0x11, 0xF0, 0x4D, 0xFF}, {0x11, 0xF0, 0x4D, 0xFF}}},
        {::runebound::dice::HandDice::PLAIN_FOREST,
         {{0x11, 0xF0, 0x4D, 0xFF}, {0x15, 0x66, 0x1D, 0xFF}}},
        {::runebound::dice::HandDice::FOREST_HILLS,
         {{0x15, 0x66, 0x1D, 0xFF}, {0x72, 0xB0, 0x34, 0xFF}}},
        {::runebound::dice::HandDice::MOUNTAINS_WATER,
         {{0x68, 0x7C, 0x7C, 0xFF}, {0x37, 0x1A, 0xCA, 0xFF}}},
        {::runebound::dice::HandDice::HILLS_PLAIN,
         {{0x72, 0xB0, 0x34, 0xFF}, {0x11, 0xF0, 0x4D, 0xFF}}},
        {::runebound::dice::HandDice::JOKER,
         {{0xFF, 0xD5, 0x00, 0xFF}, {0xFF, 0xD5, 0x00, 0xFF}}}};
}  // namespace runebound::graphics