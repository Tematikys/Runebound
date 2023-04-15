#ifndef RUNEBOUND_GRAPHICS_CONFIG_HPP_
#define RUNEBOUND_GRAPHICS_CONFIG_HPP_

#include <SDL2/SDL.h>
#include <map.hpp>
#include <string>
#include <vector>

namespace runebound::graphics {
extern const ::std::vector<::std::pair<::std::string, ::std::string>> FONTS;
extern const int HEXAGON_RADIUS;
extern const SDL_Color SELECTED_COLOR;
extern const ::std::map<::runebound::map::TypeCell, SDL_Color> CELL_FILL_COLOR;
extern const ::std::map<::runebound::map::SpecialTypeCell, SDL_Color>
    SPECIAL_COLOR;
extern const ::std::map<::runebound::AdventureType, SDL_Color> ADVENTURE_COLOR;
extern const ::std::map<::std::pair<int, int>, ::std::pair<int, int>>
    RIVER_DIRECTIONS;
extern const ::std::string WINDOW_TITLE;
extern const int WINDOWS_X_OFFSET;
extern const int WINDOWS_Y_OFFSET;
extern const int WINDOWS_WIDTH;
extern const int WINDOWS_HEIGHT;
extern const int WINDOWS_FPS;
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_CONFIG_HPP_
