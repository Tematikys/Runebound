#include "graphics.hpp"
#include <SDL2/SDL.h>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

namespace runebound::graphics {

const int SCREEN_WIDTH = 300;
const int SCREEN_HEIGHT = 300;

PolygonShape::PolygonShape(std::vector<Point> &vertexes)
    : m_vertexes(std::move(vertexes)) {
    //    int min_x = std::accumulate(
    //        m_vertexes.begin(), m_vertexes.end(), 0xFFFF,
    //        [](int x, const Point &p) { return std::min(x, p.x()); }
    //    );
    //    int max_x = std::accumulate(
    //        m_vertexes.begin(), m_vertexes.end(), 0x0000,
    //        [](int x, const Point &p) { return std::max(x, p.x()); }
    //    );
    //    int min_y = std::accumulate(
    //        m_vertexes.begin(), m_vertexes.end(), 0xFFFF,
    //        [](int y, const Point &p) { return std::min(y, p.y()); }
    //    );
    //    int max_y = std::accumulate(
    //        m_vertexes.begin(), m_vertexes.end(), 0x0000,
    //        [](int y, const Point &p) { return std::max(y, p.y()); }
    //    );
    //    m_center = Point((max_x + min_x) / 2, (max_y + min_y) / 2);
}

bool draw_filled_polygon(
    const PolygonShape &polygon,
    const SDL_Color color,
    SDL_Renderer *renderer
) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    std::vector<Point> vertices = polygon.get_vertices();
    int num_vertexes = static_cast<int>(polygon.get_number_of_vertices());
    int num_vertexes_processed = 1;

    int top_y = vertices[0].y();
    int top_index = 0;
    for (int i = 1; i < num_vertexes; ++i) {
        if (vertices[i].y() < top_y) {
            top_y = vertices[i].y();
            top_index = i;
        }
    }

    int left_index = top_index - 1;
    if (left_index < 0) {
        left_index = num_vertexes - 1;
    }
    int right_index = top_index + 1;
    if (right_index >= num_vertexes) {
        right_index = 0;
    }

    int left_x, right_x;
    left_x = right_x = (vertices[top_index].x()) << 16;

    int left_slope = 0;
    if (vertices[left_index].y() != vertices[top_index].y()) {
        left_slope =
            ((vertices[left_index].x() - vertices[top_index].x()) << 16) /
            (vertices[left_index].y() - vertices[top_index].y());
    }
    int right_slope = 0;
    if (vertices[right_index].y() != vertices[top_index].y()) {
        right_slope =
            ((vertices[right_index].x() - vertices[top_index].x()) << 16) /
            (vertices[right_index].y() - vertices[top_index].y());
    }

    int y = vertices[top_index].y();

    while (num_vertexes_processed < num_vertexes) {
        while (y < vertices[left_index].y() && y < vertices[right_index].y()) {
            SDL_RenderDrawLine(renderer, left_x >> 16, y, right_x >> 16, y);
            ++y;
            left_x += left_slope;
            right_x += right_slope;
        }

        if (vertices[left_index].y() <= y) {
            top_index = left_index;
            --left_index;
            if (left_index < 0) {
                left_index = num_vertexes - 1;
            }

            if (vertices[left_index].y() != vertices[top_index].y())
                left_slope =
                    ((vertices[left_index].x() - vertices[top_index].x())
                     << 16) /
                    (vertices[left_index].y() - vertices[top_index].y());

            left_x = (vertices[top_index].x()) << 16;
            ++num_vertexes_processed;
        }

        if (vertices[right_index].y() <= y) {
            top_index = right_index;
            ++right_index;
            if (right_index == num_vertexes) {
                right_index = 0;
            }

            if (vertices[right_index].y() != vertices[top_index].y())
                right_slope =
                    ((vertices[right_index].x() - vertices[top_index].x())
                     << 16) /
                    (vertices[right_index].y() - vertices[top_index].y());

            right_x = (vertices[top_index].x()) << 16;
            ++num_vertexes_processed;
        }

        SDL_RenderDrawLine(renderer, left_x >> 16, y, right_x >> 16, y);
    }

    return true;
};

bool SDL_init(SDL_Window *&gWindow, SDL_Renderer *&gRenderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL Error:\n"
                  << SDL_GetError() << '\n';
        return false;
    }

    if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        std::cout << "Warning: Linear texture filtering not enabled!";
    }

    gWindow = SDL_CreateWindow(
        "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );

    if (gWindow == nullptr) {
        std::cout << "Window could not be created! SDL Error:\n"
                  << SDL_GetError() << '\n';
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

    if (gRenderer == nullptr) {
        std::cout << "Renderer could not be created! SDL Error:\n"
                  << SDL_GetError() << '\n';
        return false;
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    return true;
}
}  // namespace runebound::graphics

int main(int argc, char *args[]) {
//    std::ofstream out("../out.txt");
//    std::cout.rdbuf(out.rdbuf());
//    std::cout << "Hello, World!\n";

    SDL_Window *gWindow = nullptr;
    SDL_Renderer *gRenderer = nullptr;
    if (!runebound::graphics::SDL_init(gWindow, gRenderer)) {
        std::cout << "Failed to initialize!\n";
        return 0;
    }
    bool run = true;
    SDL_Event e;
    while (run) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                run = false;
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(gRenderer);

        std::vector<runebound::graphics::Point> vertexes;

        vertexes.emplace_back(100, 0);
        vertexes.emplace_back(200, 58);
        vertexes.emplace_back(200, 173);
        vertexes.emplace_back(100, 231);
        vertexes.emplace_back(0, 173);
        vertexes.emplace_back(0, 58);

        runebound::graphics::PolygonShape poly =
            runebound::graphics::PolygonShape(vertexes);

        SDL_Color color = {.r = 255, .g = 255, .b = 255, .a = 255};

        draw_filled_polygon(poly, color, gRenderer);

        SDL_RenderPresent(gRenderer);
    }
    return 0;
}