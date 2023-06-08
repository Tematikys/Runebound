#include <graphics.hpp>
#include <iostream>

namespace runebound::graphics {
Point get_center_of_hexagon(int i, int j) {
    static const int dy = (::runebound::graphics::HEXAGON_RADIUS * 56756) >> 16;
    if (j % 2 == 0) {
        return {
            ::runebound::graphics::HEXAGON_RADIUS * (2 + j * 3) / 2,
            dy * (1 + 2 * i)};
    }
    return {
        ::runebound::graphics::HEXAGON_RADIUS * (2 + j * 3) / 2,
        dy * 2 * (1 + i)};
}

bool SDL_init(SDL_Window *&window, SDL_Renderer *&renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL Error:\n"
                    << SDL_GetError() << std::endl;
        return false;
    }
    if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") == 0U) {
        std::cout << "Warning: Linear texture filtering not enabled!"
                    << std::endl;
    }
    window = SDL_CreateWindow(
        WINDOW_TITLE.c_str(), WINDOW_X_OFFSET, WINDOW_Y_OFFSET, WINDOW_WIDTH,
        WINDOW_HEIGHT, SDL_WINDOW_SHOWN
    );
    if (window == nullptr) {
        std::cout << "Window could not be created! SDL Error:\n"
                    << SDL_GetError() << std::endl;
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cout << "Renderer could not be created! SDL Error:\n"
                    << SDL_GetError() << std::endl;
        return false;
    }
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    const int imgFlags = IMG_INIT_PNG;
    if ((IMG_Init(imgFlags) & imgFlags) == 0) {
        std::cout << "SDL_image could not initialize! SDL_image Error:\n"
                    << IMG_GetError() << std::endl;
        return false;
    }
    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error:\n"
                    << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}

void update_mouse_pos(Point &pos) {
    int x = 0;
    int y = 0;
    SDL_GetMouseState(&x, &y);
    pos = Point(x, y);
}

bool load_font(TTF_Font *&font, const std::string &path, int font_size) {
    font = TTF_OpenFont(path.c_str(), font_size);
    if (font == nullptr) {
        std::cout << "Failed to load font! SDL_ttf Error:\n"
                    << TTF_GetError() << std::endl;
        return false;
    }
    return true;
}
}  // namespace runebound::graphics