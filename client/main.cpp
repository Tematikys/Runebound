#include <SDL2/SDL.h>
#include <graphics.hpp>
#include <graphics_board.hpp>
#include <iostream>

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