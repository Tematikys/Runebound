#include <graphics.hpp>
#include <iostream>

namespace runebound::graphics {
bool SDL_init(
    SDL_Window *&gWindow,
    SDL_Renderer *&gRenderer,
    const char *title,
    int x_pos,
    int y_pos,
    int width,
    int height
) {
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ::std::cout << "SDL could not initialize! SDL Error:\n"
                    << SDL_GetError() << '\n';
        return false;
    }

    // enable linear texture filtering
    if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") == 0U) {
        ::std::cout << "Warning: Linear texture filtering not enabled!";
    }

    // create window
    gWindow =
        SDL_CreateWindow(title, x_pos, y_pos, width, height, SDL_WINDOW_SHOWN);

    // report error if appeared
    if (gWindow == nullptr) {
        ::std::cout << "Window could not be created! SDL Error:\n"
                    << SDL_GetError() << '\n';
        return false;
    }

    // create renderer
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    // report errors
    if (gRenderer == nullptr) {
        ::std::cout << "Renderer could not be created! SDL Error:\n"
                    << SDL_GetError() << '\n';
        return false;
    }

    // set default render color
    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // return true, if initialization was successful
    return true;
}
}  // namespace runebound::graphics