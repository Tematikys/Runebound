#include <graphics_config.hpp>

namespace runebound::graphics {
bool SHOW_CLIENT_DEBUG_INFO{};
bool SHOW_TEXTURE_DEBUG_INFO{};
bool SHOW_SHOP_DEBUG_INFO{};
int HEXAGON_RADIUS{};
SDL_Color SELECTED_COLOR{};
std::map<::runebound::map::TypeCell, SDL_Color> CELL_FILL_COLOR{};
std::map<::runebound::map::SpecialTypeCell, SDL_Color> SPECIAL_COLOR{};
std::map<::runebound::AdventureType, SDL_Color> ADVENTURE_COLOR{};
std::map<::runebound::AdventureType, SDL_Color> USED_ADVENTURE_COLOR{};
std::string WINDOW_TITLE{};
int WINDOW_X_OFFSET{};
int WINDOW_Y_OFFSET{};
int WINDOW_WIDTH{};
int WINDOW_HEIGHT{};
int WINDOW_FPS{};
std::map<::runebound::dice::HandDice, std::pair<SDL_Color, SDL_Color>>
    DICE_COLOR{};

std::vector<std::pair<std::string, std::string>> FONTS{};
std::vector<std::pair<std::string, std::string>> IMAGES{};

const std::map<::runebound::character::StandardCharacter, std::string>
    STANDARD_CHARACTER_TO_STRING = {
        {character::StandardCharacter::LISSA, "Lissa"},
        {character::StandardCharacter::CORBIN, "Corbin"},
        {character::StandardCharacter::ELDER_MOK, "Elder Mok"},
        {character::StandardCharacter::LAUREL_FROM_BLOODWOOD,
         "Laurel from Bloodwood"},
        {character::StandardCharacter::LORD_HAWTHORNE, "Lord Hawthorne"},
        {character::StandardCharacter::MASTER_THORN, "Master Thorn"},
        {character::StandardCharacter::NONE, "None"}};

const std::map<std::string, std::string> CHARACTER_NAMES_WITH_DASH = {
    {"Lissa", "Lissa"},
    {"Corbin", "Corbin"},
    {"Elder Mok", "Elder-Mok"},
    {"Laurel from Bloodwood", "Laurel-from-Bloodwood"},
    {"Lord Hawthorne", "Lord-Hawthorne"},
    {"Master Thorn", "Master-Thorn"}};
const std::map<::runebound::fight::HandFightTokens, std::string>
    HAND_FIGHT_TOKENS_TO_STR = {
        {fight::HandFightTokens::PHYSICAL_DAMAGE, "damage"},
        {fight::HandFightTokens::MAGICAL_DAMAGE, "magic"},
        {fight::HandFightTokens::DEXTERITY, "dexterity"},
        {fight::HandFightTokens::HIT, "hit"},
        {fight::HandFightTokens::ENEMY_DAMAGE, "skull"},
        {fight::HandFightTokens::DOUBLING, "double"},
        {fight::HandFightTokens::SHIELD, "shield"},
        {fight::HandFightTokens::NOTHING, "none"},
};
const std::map<::runebound::map::SpecialTypeCell, std::string> SPECIAL_TO_STR =
    {{::runebound::map::SpecialTypeCell::SANCTUARY, "Sanctuary"},
     {::runebound::map::SpecialTypeCell::FORTRESS, "Fortress"},
     {::runebound::map::SpecialTypeCell::SETTLEMENT, "Settlement"},
     {::runebound::map::SpecialTypeCell::NOTHING, ""}};
const std::map<std::pair<int, int>, std::pair<int, int>> RIVER_DIRECTIONS = {
    {{-1, 0}, {3, 4}}, {{-1, -1}, {2, 3}}, {{-1, 1}, {4, 5}},
    {{1, 0}, {0, 1}},  {{1, -1}, {1, 2}},  {{1, 1}, {5, 0}}};
}  // namespace runebound::graphics