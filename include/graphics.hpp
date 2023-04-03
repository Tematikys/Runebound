#ifndef RUNEBOUND_GRAPHICS_HPP_
#define RUNEBOUND_GRAPHICS_HPP_

#include <SDL2/SDL.h>
#include <utility>

namespace runebound::graphics {
// SDL init function
bool SDL_init(
    SDL_Window *&gWindow,
    SDL_Renderer *&gRenderer,
    const char *title,
    int x_pos,
    int y_pos,
    int width,
    int height
);

void update_mouse_pos(::std::pair<int, int> &pos);
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_HPP_