#include <SDL2/SDL.h>
#include <algorithm>
#include <fstream>
#include <graphics.hpp>
#include <iostream>
#include <map.hpp>
#include <map_cell.hpp>
#include <tuple>
#include <vector>

namespace runebound::graphics {

// screen size parameters
const int SCREEN_WIDTH = 640;

const int SCREEN_HEIGHT = 480;

const int HEXAGON_RADIUS = 50;

// initialize polygon's sides coefficients
void PolygonShape::init_side_coefficients() {
    for (::std::size_t i = 0; i < m_vertexes.size(); ++i) {
        const int x1 = m_vertexes[(i + 1) % 6].x();
        const int y1 = m_vertexes[(i + 1) % 6].y();
        const int x2 = m_vertexes[i].x();
        const int y2 = m_vertexes[i].y();
        m_side_coefficients.emplace_back(y1 - y2, x2 - x1, x1 * y2 - x2 * y1);
    }
}

// check that point is inside of polygon
bool PolygonShape::in_bounds(Point dot) const {
    return ::std::all_of(
        m_side_coefficients.begin(), m_side_coefficients.end(),
        [&](::std::tuple<int, int, int> coefficients) {
            auto [a, b, c] = coefficients;
            return dot.x() * a + dot.y() * b + c < 0;
        }
    );
}

// hexagon constructor from given center and radius
HexagonShape::HexagonShape(Point center, int radius) {
    // rounded multiplication by cos(pi/6)
    const int dx = (radius * 56756) >> 16;
    m_vertexes.emplace_back(center.x(), center.y() - radius);
    m_vertexes.emplace_back(center.x() + dx, center.y() - radius / 2);
    m_vertexes.emplace_back(center.x() + dx, center.y() + radius / 2);
    m_vertexes.emplace_back(center.x(), center.y() + radius);
    m_vertexes.emplace_back(center.x() - dx, center.y() + radius / 2);
    m_vertexes.emplace_back(center.x() - dx, center.y() - radius / 2);
    init_side_coefficients();
}

// draw colored polygon function definition
bool draw_filled_polygon(
    const PolygonShape &polygon,
    SDL_Color color,
    SDL_Renderer *renderer
) {
    // set required color
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // get and set necessary variables
    ::std::vector<Point> vertexes = polygon.get_vertexes();
    const int num_vertexes = static_cast<int>(polygon.get_number_of_vertexes());
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
    int left_x{};
    int right_x{};
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
            if (vertexes[left_index].y() != vertexes[top_index].y()) {
                left_slope =
                    ((vertexes[left_index].x() - vertexes[top_index].x())
                     << 16) /
                    (vertexes[left_index].y() - vertexes[top_index].y());
            }

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

            if (vertexes[right_index].y() != vertexes[top_index].y()) {
                right_slope =
                    ((vertexes[right_index].x() - vertexes[top_index].x())
                     << 16) /
                    (vertexes[right_index].y() - vertexes[top_index].y());
            }

            right_x = (vertexes[top_index].x()) << 16;
            ++num_vertexes_processed;
        }

        // draw connecting line
        SDL_RenderDrawLine(renderer, left_x >> 16, y, right_x >> 16, y);
    }

    // if rendering was successful, return true
    return true;
}

// draw colored polygon function definition
bool draw_polygon_border(
    const PolygonShape &polygon,
    SDL_Color color,
    SDL_Renderer *renderer
) {
    // set required color
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // draw every edge of polygon except last
    for (::std::size_t i = 1; i < polygon.get_number_of_vertexes(); ++i) {
        SDL_RenderDrawLine(
            renderer, polygon.get_vertex(i - 1).x(),
            polygon.get_vertex(i - 1).y(), polygon.get_vertex(i).x(),
            polygon.get_vertex(i).y()
        );
    }

    // draw last edge
    SDL_RenderDrawLine(
        renderer, polygon.get_vertex(0).x(), polygon.get_vertex(0).y(),
        polygon.get_vertex(polygon.get_number_of_vertexes() - 1).x(),
        polygon.get_vertex(polygon.get_number_of_vertexes() - 1).y()
    );

    return true;
}

// draw map function definition
bool draw_map(const ::runebound::map::Map &map, SDL_Renderer *renderer) {
    // iterate by every cell
    for (int row = 0; row < ::runebound::map::StandartHeight; ++row) {
        for (int col = 0; col < ::runebound::map::StandartWidth; ++col) {
            SDL_Color color;

            // get necessary color due to the type of cell
            switch (map.get_cell_map(row, col).get_type_cell()) {
                case ::runebound::map::TypeCell::WATER:
                    color = {0x00, 0x00, 0xFF, 0xFF};
                    break;
                case ::runebound::map::TypeCell::FOREST:
                    color = {0x00, 0xFF, 0x00, 0xFF};
                    break;
                case ::runebound::map::TypeCell::MOUNTAINS:
                    color = {0x77, 0x77, 0x77, 0xFF};
                    break;
                case ::runebound::map::TypeCell::HILLS:
                    color = {0x00, 0x77, 0x00, 0xFF};
                    break;
                case ::runebound::map::TypeCell::PLAIN:
                    color = {0x77, 0xFF, 0x77, 0xFF};
                    break;
            }

            // supportive variable
            const int dx = (HEXAGON_RADIUS * 56756) >> 16;

            // set coordinates due to parity of row
            HexagonShape hex;
            if (row % 2 == 0) {
                hex = HexagonShape(
                    Point(
                        dx * 2 * (1 + col), HEXAGON_RADIUS * (2 + row * 3) / 2
                    ),
                    HEXAGON_RADIUS
                );
            } else {
                hex = HexagonShape(
                    Point(
                        dx * (1 + 2 * col), HEXAGON_RADIUS * (2 + row * 3) / 2
                    ),
                    HEXAGON_RADIUS
                );
            }

            // draw polygon
            draw_filled_polygon(hex, color, renderer);

            // draw border for better contrast
            draw_polygon_border(
                hex, SDL_Color{0x00, 0x00, 0x00, 0xFF}, renderer
            );
        }
    }

    return true;
}

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
        "Runebound-v0.0.2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
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
    while (run) {
        // process events
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                run = false;
            }
        }

        /// SDL_WarpMouseInWindow(gWindow, 320, 240);

        // drawing test hexagon
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        const ::runebound::map::Map map;
        ::runebound::graphics::draw_map(map, gRenderer);

        int x{};
        int y{};
        SDL_GetMouseState(&x, &y);
        for (int row = 0; row < ::runebound::map::StandartHeight; ++row) {
            for (int col = 0; col < ::runebound::map::StandartWidth; ++col) {
                const SDL_Color color = {0xFF, 0xF7, 0x00, 0xFF};
                const int dx =
                    (::runebound::graphics::HEXAGON_RADIUS * 56756) >> 16;
                ::runebound::graphics::HexagonShape hex;
                if (row % 2 == 0) {
                    hex = ::runebound::graphics::HexagonShape(
                        ::runebound::graphics::Point(
                            dx * 2 * (1 + col),
                            ::runebound::graphics::HEXAGON_RADIUS *
                                (2 + row * 3) / 2
                        ),
                        ::runebound::graphics::HEXAGON_RADIUS
                    );
                } else {
                    hex = ::runebound::graphics::HexagonShape(
                        ::runebound::graphics::Point(
                            dx * (1 + 2 * col),
                            ::runebound::graphics::HEXAGON_RADIUS *
                                (2 + row * 3) / 2
                        ),
                        ::runebound::graphics::HEXAGON_RADIUS
                    );
                }
                if (hex.in_bounds(::runebound::graphics::Point(x, y))) {
                    draw_filled_polygon(hex, color, gRenderer);
                }
            }
        }

        SDL_RenderPresent(gRenderer);
    }
    return 0;
}