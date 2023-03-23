#include "graphics.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

namespace runebound::graphics {

PolygonShape::PolygonShape(const std::vector<Point> &vertices_) {
    int minX = 0xFFFF, minY = 0xFFFF, maxX = 0, maxY = 0;
    length = vertices_.size();
    vertices = vertices_;
    for (std::size_t i = 0; i < length; i++) {
        vertices[i] = Point(vertices[i].x, vertices[i].y);
        if (vertices[i].x > maxX)
            maxX = vertices[i].x;
        if (vertices[i].x < minX)
            minX = vertices[i].x;
        if (vertices[i].y > maxY)
            maxY = vertices[i].y;
        if (vertices[i].y < minY)
            minY = vertices[i].y;
    }
    center.x = minX + ((maxX - minX) / 2);
    center.y = minY + ((maxY - minY) / 2);
}

bool DrawFilledPolygon(
    const PolygonShape &polygon,
    const SDL_Color color,
    SDL_Renderer *renderer
) {
    int topY, topCnt, leftCnt, rightCnt, startX, endX, cntY, leftSlope,
        rightSlope, cnt;
    int numVertices = static_cast<int>(polygon.getNumberOfVertices());
    int numVerticesProc = 1;

    Point center = polygon.getCenter();
    std::vector<Point> vertices = polygon.getVertices();

    topY = vertices[0].y;
    topCnt = 0;

    for (cnt = 1; cnt < numVertices; cnt++) {
        if (vertices[cnt].y < topY) {
            topY = vertices[cnt].y;
            topCnt = cnt;
        }
    }

    leftCnt = topCnt - 1;
    if (leftCnt < 0)
        leftCnt = numVertices - 1;

    rightCnt = topCnt + 1;
    if (rightCnt >= numVertices)

        rightCnt = 0;

    startX = endX = (vertices[topCnt].x + center.x) << 16;
    cntY = vertices[topCnt].y;

    if (vertices[leftCnt].y != vertices[topCnt].y)
        leftSlope = ((vertices[leftCnt].x - vertices[topCnt].x) << 16) /
                    (vertices[leftCnt].y - vertices[topCnt].y);
    if (vertices[rightCnt].y != vertices[topCnt].y)
        rightSlope = ((vertices[rightCnt].x - vertices[topCnt].x) << 16) /
                     (vertices[rightCnt].y - vertices[topCnt].y);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    while (numVerticesProc < numVertices) {
        while (cntY < vertices[leftCnt].y && cntY < vertices[rightCnt].y) {
            SDL_RenderDrawLine(
                renderer, startX >> 16, cntY + center.y, endX >> 16,
                cntY + center.y
            );

            cntY++;
            startX += leftSlope;
            endX += rightSlope;
        }

        if (vertices[leftCnt].y <= cntY) {
            topCnt = leftCnt;
            leftCnt--;
            if (leftCnt < 0)
                leftCnt = numVertices - 1;
            if (vertices[leftCnt].y != vertices[topCnt].y)

                leftSlope = ((vertices[leftCnt].x - vertices[topCnt].x) << 16) /
                            (vertices[leftCnt].y - vertices[topCnt].y);

            startX = (vertices[topCnt].x + center.x) << 16;
            numVerticesProc++;
        }

        if (vertices[rightCnt].y <= cntY) {
            topCnt = rightCnt;
            rightCnt++;
            if (rightCnt == numVertices)

                rightCnt = 0;
            if (vertices[rightCnt].y != vertices[topCnt].y)

                rightSlope =
                    ((vertices[rightCnt].x - vertices[topCnt].x) << 16) /
                    (vertices[rightCnt].y - vertices[topCnt].y);

            endX = (vertices[topCnt].x + center.x) << 16;
            numVerticesProc++;
        }
        SDL_RenderDrawLine(
            renderer, startX >> 16, cntY + center.y, endX >> 16, cntY + center.y
        );
    }

    return true;
};

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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

int main(int /*argc*/, char * /*args*/[]) {
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

        std::vector<runebound::graphics::Point> vertices;

        vertices.emplace_back(150, 50);
        vertices.emplace_back(200, 79);
        vertices.emplace_back(200, 137);
        vertices.emplace_back(150, 166);
        vertices.emplace_back(100, 137);
        vertices.emplace_back(100, 79);

        runebound::graphics::PolygonShape poly =
            runebound::graphics::PolygonShape(vertices);

        SDL_Color color = {.r = 255, .g = 255, .b = 255, .a = 255};

        DrawFilledPolygon(poly, color, gRenderer);

        SDL_RenderPresent(gRenderer);
    }
    return 0;
}