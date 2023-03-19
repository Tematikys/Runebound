#include <SDL2/SDL.h>
#include <graphics.hpp>
#include <iostream>
#include <map.hpp>

namespace runebound::graphics {
// SDL init function
bool SDL_init(SDL_Window *&gWindow, SDL_Renderer *&gRenderer) {
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
    gWindow = SDL_CreateWindow(
        "Runebound-v0.0.5", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );

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

int main(int /*argc*/, char * /*args*/[]) {
    // declare window and renderer
    SDL_Window *gWindow = nullptr;
    SDL_Renderer *gRenderer = nullptr;
    // init
    if (!::runebound::graphics::SDL_init(gWindow, gRenderer)) {
        ::std::cout << "Failed to initialize!\n";
        return 0;
    }

    // run flag
    bool run = true;
    // event handler
    SDL_Event e;
    const ::runebound::map::Map map;
    ::runebound::graphics::Board board(map);

    // variables for mouse position
    int x{};
    int y{};
    while (run) {
        // process events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                run = false;
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        SDL_GetMouseState(&x, &y);

        board.set_selected_hexagon(0xFFFF);
        if (auto index = board.in_bounds(::runebound::graphics::Point(x, y))) {
            board.set_selected_hexagon(index.value());
        }

        board.render(gRenderer);
        SDL_RenderPresent(gRenderer);
    }
    return 0;
}