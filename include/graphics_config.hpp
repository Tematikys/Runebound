#ifndef RUNEBOUND_GRAPHICS_CONFIG_HPP_
#define RUNEBOUND_GRAPHICS_CONFIG_HPP_

#include <SDL2/SDL.h>
#include <map.hpp>
#include <string>
#include <vector>

namespace runebound::graphics {
extern const ::std::vector<::std::pair<::std::string, ::std::string>> FONTS;
extern const ::std::vector<::std::pair<::std::string, ::std::string>> IMAGES;
extern const int HEXAGON_RADIUS;
extern const SDL_Color SELECTED_COLOR;
extern const ::std::map<::runebound::map::TypeCell, SDL_Color> CELL_FILL_COLOR;
extern const ::std::map<::runebound::map::SpecialTypeCell, SDL_Color>
    SPECIAL_COLOR;
extern const ::std::map<::runebound::AdventureType, SDL_Color> ADVENTURE_COLOR;
extern const ::std::map<::runebound::AdventureType, SDL_Color>
    USED_ADVENTURE_COLOR;
extern const ::std::map<::std::pair<int, int>, ::std::pair<int, int>>
    RIVER_DIRECTIONS;
extern const ::std::string WINDOW_TITLE;
extern const int WINDOW_X_OFFSET;
extern const int WINDOW_Y_OFFSET;
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern const int WINDOW_FPS;
extern const ::std::
    map<::runebound::dice::HandDice, ::std::pair<SDL_Color, SDL_Color>>
        DICE_COLOR;
enum class HorizontalButtonTextureAlign { NONE, LEFT, CENTER, RIGHT };
enum class VerticalButtonTextureAlign { NONE, TOP, CENTER, BOTTOM };
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_CONFIG_HPP_
