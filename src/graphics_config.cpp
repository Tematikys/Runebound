#include <graphics_config.hpp>

namespace runebound::graphics {
bool SHOW_CLIENT_DEBUG_INFO = false;
bool SHOW_TEXTURE_DEBUG_INFO = false;
bool SHOW_SHOP_DEBUG_INFO = false;
const ::std::vector<::std::pair<::std::string, ::std::string>> FONTS = {
    {"data/fonts/FreeMono.ttf", "FreeMono"}};
const ::std::vector<::std::pair<::std::string, ::std::string>> IMAGES = {
    {"data/images/heart20.png", "heart20"},
    {"data/images/coin20.png", "coin20"},
    {"data/images/fight_token.png", "fight_token"},
    {"data/images/fight_token_selected.png", "fight_token_selected"},
    {"data/images/shield.png", "shield"},
    {"data/images/shield_init.png", "shield_init"},
    {"data/images/damage.png", "damage"},
    {"data/images/damage_init.png", "damage_init"},
    {"data/images/magic.png", "magic"},
    {"data/images/magic_init.png", "magic_init"},
    {"data/images/none.png", "none"},
    {"data/images/none_init.png", "none_init"},
    {"data/images/double.png", "double"},
    {"data/images/double_init.png", "double_init"},
    {"data/images/dexterity.png", "dexterity"},
    {"data/images/dexterity_init.png", "dexterity_init"},
    {"data/images/skull.png", "skull"},
    {"data/images/skull_init.png", "skull_init"},
    {"data/images/shield32.png", "shield32"},
    {"data/images/shield_init32.png", "shield_init32"},
    {"data/images/damage32.png", "damage32"},
    {"data/images/damage_init32.png", "damage_init32"},
    {"data/images/magic32.png", "magic32"},
    {"data/images/magic_init32.png", "magic_init32"},
    {"data/images/none32.png", "none32"},
    {"data/images/none_init32.png", "none_init32"},
    {"data/images/double32.png", "double32"},
    {"data/images/double_init32.png", "double_init32"},
    {"data/images/dexterity32.png", "dexterity32"},
    {"data/images/dexterity_init32.png", "dexterity_init32"},
    {"data/images/skull32.png", "skull32"},
    {"data/images/skull_init32.png", "skull_init32"},
    {"data/images/LISSA.png", "Lissa"},
    {"data/images/LISSA40.png", "Lissa40"},
    {"data/images/CORBIN.png", "Corbin"},
    {"data/images/CORBIN40.png", "Corbin40"},
    {"data/images/ELDER_MOK.png", "Elder Mok"},
    {"data/images/ELDER_MOK40.png", "Elder Mok40"},
    {"data/images/LAUREL_FROM_BLOODWOOD.png", "Laurel from Bloodwood"},
    {"data/images/LAUREL_FROM_BLOODWOOD40.png", "Laurel from Bloodwood40"},
    {"data/images/LORD_HAWTHORNE.png", "Lord Hawthorne"},
    {"data/images/LORD_HAWTHORNE40.png", "Lord Hawthorne40"},
    {"data/images/MASTER_THORN.png", "Master Thorn"},
    {"data/images/MASTER_THORN40.png", "Master Thorn40"}};
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
const ::std::map<::runebound::AdventureType, SDL_Color> USED_ADVENTURE_COLOR = {
    {::runebound::AdventureType::MEETING, {0x00, 0x00, 0x00, 0xFF}},
    {::runebound::AdventureType::RESEARCH, {0x00, 0x00, 0x00, 0xFF}},
    {::runebound::AdventureType::FIGHT, {0x00, 0x00, 0x00, 0xFF}}};
const ::std::map<::std::pair<int, int>, ::std::pair<int, int>>
    RIVER_DIRECTIONS = {{{-1, 0}, {3, 4}}, {{-1, -1}, {2, 3}},
                        {{-1, 1}, {4, 5}}, {{1, 0}, {0, 1}},
                        {{1, -1}, {1, 2}}, {{1, 1}, {5, 0}}};
const ::std::string WINDOW_TITLE = "Runebound-0.2";
const int WINDOW_X_OFFSET = 25;
const int WINDOW_Y_OFFSET = 50;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
const int WINDOW_FPS = 10;
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