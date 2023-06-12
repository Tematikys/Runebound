#ifndef RUNEBOUND_GRAPHICS_CONFIG_HPP_
#define RUNEBOUND_GRAPHICS_CONFIG_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <fight.hpp>
#include <map.hpp>
#include <string>
#include <vector>

namespace runebound::graphics {
extern bool SHOW_CLIENT_DEBUG_INFO;
extern bool SHOW_TEXTURE_DEBUG_INFO;
extern bool SHOW_SHOP_DEBUG_INFO;
extern int HEXAGON_RADIUS;
extern SDL_Color SELECTED_COLOR;
extern std::map<::runebound::map::TypeCell, SDL_Color> CELL_FILL_COLOR;
extern std::map<::runebound::map::SpecialTypeCell, SDL_Color> SPECIAL_COLOR;
extern std::map<::runebound::AdventureType, SDL_Color> ADVENTURE_COLOR;
extern std::map<::runebound::AdventureType, SDL_Color> USED_ADVENTURE_COLOR;
extern std::string WINDOW_TITLE;
extern int WINDOW_X_OFFSET;
extern int WINDOW_Y_OFFSET;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;
extern int WINDOW_FPS;
extern std::map<::runebound::dice::HandDice, std::pair<SDL_Color, SDL_Color>>
    DICE_COLOR;

extern std::vector<std::pair<std::string, std::string>> FONTS;
extern std::vector<std::pair<std::string, std::string>> IMAGES;

extern const std::map<std::string, std::string> CHARACTER_NAMES_WITH_DASH;
extern const std::map<::runebound::fight::HandFightTokens, std::string>
    HAND_FIGHT_TOKENS_TO_STR;
extern const std::map<std::pair<int, int>, std::pair<int, int>>
    RIVER_DIRECTIONS;
enum class HorizontalButtonTextureAlign { NONE, LEFT, CENTER, RIGHT };
enum class VerticalButtonTextureAlign { NONE, TOP, CENTER, BOTTOM };
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_CONFIG_HPP_
