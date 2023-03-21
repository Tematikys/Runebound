#ifndef RUNEBOUND_GRAPHICS_HPP_
#define RUNEBOUND_GRAPHICS_HPP_

#include <SDL2/SDL.h>

namespace runebound::graphics {
// graphic constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// SDL init function
bool SDL_init(SDL_Window *&gWindow, SDL_Renderer *&gRenderer);
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_HPP_