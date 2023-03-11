#include "graphics.hpp"
#include <SDL2/SDL.h>
#include <fstream>
#include <iostream>
#include <vector>

namespace runebound::graphics {

// screen size parameters
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

// draw colored polygon function
bool draw_filled_polygon(
    const PolygonShape &polygon,
    const SDL_Color color,
    SDL_Renderer *renderer
) {
    // set required color
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // get and set necessary variables
    ::std::vector<Point> vertexes = polygon.get_vertexes();
    int num_vertexes = static_cast<int>(polygon.get_number_of_vertexes());
    int num_vertexes_processed = 1;

    // find the highest vertex
    int top_y = vertexes[0].y();
    int top_index = 0;
    for (int i = 1; i < num_vertexes; ++i) {
        if (vertexes[i].y() < top_y) {
            top_y = vertexes[i].y();
            top_index = i;
        }
    }

    // left-side vertex
    int left_index = top_index - 1;
    if (left_index < 0) {
        left_index = num_vertexes - 1;
    }
    // right-side vertex
    int right_index = top_index + 1;
    if (right_index >= num_vertexes) {
        right_index = 0;
    }

    // left, right-side x
    int left_x, right_x;
    left_x = right_x = (vertexes[top_index].x()) << 16;

    // left dx slope
    int left_slope = 0;
    if (vertexes[left_index].y() != vertexes[top_index].y()) {
        left_slope =
            ((vertexes[left_index].x() - vertexes[top_index].x()) << 16) /
            (vertexes[left_index].y() - vertexes[top_index].y());
    }
    // right dx slope
    int right_slope = 0;
    if (vertexes[right_index].y() != vertexes[top_index].y()) {
        right_slope =
            ((vertexes[right_index].x() - vertexes[top_index].x()) << 16) /
            (vertexes[right_index].y() - vertexes[top_index].y());
    }

    // y coordinate
    int y = vertexes[top_index].y();

    // until all the vertices are drawn, draw
    while (num_vertexes_processed < num_vertexes) {
        // while y is above side vertexes, draw horizontal lines
        while (y < vertexes[left_index].y() && y < vertexes[right_index].y()) {
            // draw line
            SDL_RenderDrawLine(renderer, left_x >> 16, y, right_x >> 16, y);
            // lower y
            ++y;
            // shift xs
            left_x += left_slope;
            right_x += right_slope;
        }

        // if y coordinate is on the same level as left vertex, go to next
        if (vertexes[left_index].y() <= y) {
            // make top vertex equal left
            top_index = left_index;
            // go to next vertex
            --left_index;
            // module
            if (left_index < 0) {
                left_index = num_vertexes - 1;
            }

            // change slope if needed
            if (vertexes[left_index].y() != vertexes[top_index].y())
                left_slope =
                    ((vertexes[left_index].x() - vertexes[top_index].x())
                     << 16) /
                    (vertexes[left_index].y() - vertexes[top_index].y());

            // << 16 is used for rounding
            left_x = (vertexes[top_index].x()) << 16;
            // +1 processed vertex
            ++num_vertexes_processed;
        }

        // same for right side
        if (vertexes[right_index].y() <= y) {
            top_index = right_index;
            ++right_index;
            if (right_index == num_vertexes) {
                right_index = 0;
            }

            if (vertexes[right_index].y() != vertexes[top_index].y())
                right_slope =
                    ((vertexes[right_index].x() - vertexes[top_index].x())
                     << 16) /
                    (vertexes[right_index].y() - vertexes[top_index].y());

            right_x = (vertexes[top_index].x()) << 16;
            ++num_vertexes_processed;
        }

        // draw connecting line
        SDL_RenderDrawLine(renderer, left_x >> 16, y, right_x >> 16, y);
    }

    // if rendering was successful, return true
    return true;
};

// SDL init function
bool SDL_init(SDL_Window *&gWindow, SDL_Renderer *&gRenderer) {
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ::std::cout << "SDL could not initialize! SDL Error:\n"
                    << SDL_GetError() << '\n';
        return false;
    }

    // enable linear texture filtering
    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        ::std::cout << "Warning: Linear texture filtering not enabled!";
    }

    // create window
    gWindow = SDL_CreateWindow(
        "Runebound-v001", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
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

int main(int argc, char *args[]) {
    // connect fstream because standard iostream does not work properly
    ::std::ofstream out("../out.txt");
    ::std::cout.rdbuf(out.rdbuf());

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
    while (run) {
        // process events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                run = false;
            }
        }

        // drawing test hexagon
        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(gRenderer);

        ::std::vector<::runebound::graphics::Point> vertexes;

        vertexes.emplace_back(100, 0);
        vertexes.emplace_back(200, 58);
        vertexes.emplace_back(200, 173);
        vertexes.emplace_back(100, 231);
        vertexes.emplace_back(0, 173);
        vertexes.emplace_back(0, 58);

        ::runebound::graphics::PolygonShape poly =
            ::runebound::graphics::PolygonShape(vertexes);

        SDL_Color color = {.r = 255, .g = 255, .b = 255, .a = 255};

        draw_filled_polygon(poly, color, gRenderer);

        SDL_RenderPresent(gRenderer);
    }
    return 0;
}