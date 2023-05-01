#include <graphics.hpp>
#include <iostream>

namespace runebound::graphics {
void Texture::free() {
    if (m_texture != nullptr) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
        m_width = 0;
        m_height = 0;
    }
}

void Texture::render(
    SDL_Renderer *renderer,
    int x,
    int y,
    SDL_Rect *clip,
    double angle,
    SDL_Point *center,
    SDL_RendererFlip flip
) const {
    SDL_Rect renderQuad = {x, y, m_width, m_height};
    if (clip != nullptr) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(
        renderer, m_texture, clip, &renderQuad, angle, center, flip
    );
}

bool Texture::load_image_from_file(
    SDL_Renderer *renderer,
    const ::std::string &path
) {
    free();

    SDL_Texture *new_texture = nullptr;

    SDL_Surface *loaded_surface = IMG_Load(path.c_str());
    if (loaded_surface == nullptr) {
        ::std::cout << "Unable to load image! SDL_image Error:\n"
                    << path << ' ' << IMG_GetError() << ::std::endl;
    } else {
        SDL_SetColorKey(
            loaded_surface, SDL_TRUE,
            SDL_MapRGB(loaded_surface->format, 0xFF, 0xFF, 0xFF)
        );

        new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
        if (new_texture == nullptr) {
            ::std::cout << "Unable to create texture from! SDL Error:\n"
                        << path << ' ' << SDL_GetError() << ::std::endl;
        } else {
            m_width = loaded_surface->w;
            m_height = loaded_surface->h;
        }
        SDL_FreeSurface(loaded_surface);
    }

    m_texture = new_texture;
    return m_texture != nullptr;
}

bool Texture::load_text_from_string(
    SDL_Renderer *renderer,
    TTF_Font *font,
    const ::std::string &text,
    SDL_Color color
) {
    free();

    if (text.length() == 0) {
        return true;
    }
    SDL_Surface *text_surface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (text_surface == nullptr) {
        ::std::cout << "Unable to render text surface! SDL_ttf Error:\n"
                    << TTF_GetError() << ::std::endl;
    } else {
        m_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        if (m_texture == nullptr) {
            ::std::cout
                << "Unable to create texture from rendered text! SDL Error:\n"
                << SDL_GetError() << ::std::endl;
        } else {
            m_width = text_surface->w;
            m_height = text_surface->h;
        }
        SDL_FreeSurface(text_surface);
    }

    return m_texture != nullptr;
}

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
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        ::std::cout << "SDL could not initialize! SDL Error:\n"
                    << SDL_GetError() << ::std::endl;
        return false;
    }

    // enable linear texture filtering
    if (SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1") == 0U) {
        ::std::cout << "Warning: Linear texture filtering not enabled!"
                    << ::std::endl;
    }

    // create window
    window = SDL_CreateWindow(
        WINDOW_TITLE.c_str(), WINDOW_X_OFFSET, WINDOW_Y_OFFSET, WINDOW_WIDTH,
        WINDOW_HEIGHT, SDL_WINDOW_SHOWN
    );

    // report error if appeared
    if (window == nullptr) {
        ::std::cout << "Window could not be created! SDL Error:\n"
                    << SDL_GetError() << ::std::endl;
        return false;
    }

    // create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // report errors
    if (renderer == nullptr) {
        ::std::cout << "Renderer could not be created! SDL Error:\n"
                    << SDL_GetError() << ::std::endl;
        return false;
    }

    // set default render color
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    // flags for SDL_Image
    const int imgFlags = IMG_INIT_PNG;
    if ((IMG_Init(imgFlags) & imgFlags) == 0) {
        ::std::cout << "SDL_image could not initialize! SDL_image Error:\n"
                    << IMG_GetError() << ::std::endl;
        return false;
    }

    // initialize TTF
    if (TTF_Init() == -1) {
        ::std::cout << "SDL_ttf could not initialize! SDL_ttf Error:\n"
                    << TTF_GetError() << ::std::endl;
        return false;
    }

    // return true, if initialization was successful
    return true;
}

void update_mouse_pos(Point &pos) {
    int x = 0;
    int y = 0;
    SDL_GetMouseState(&x, &y);
    pos = Point(x, y);
}

bool generate_text(
    SDL_Renderer *renderer,
    Texture &texture,
    const ::std::string &text,
    TTF_Font *font,
    SDL_Color color
) {
    if (!texture.load_text_from_string(renderer, font, text, color)) {
        ::std::cout << "Failed to render text texture!" << ::std::endl;
        return false;
    }

    return true;
}

bool load_font(TTF_Font *&font, const ::std::string &path, int font_size) {
    font = TTF_OpenFont(path.c_str(), font_size);
    if (font == nullptr) {
        ::std::cout << "Failed to load font! SDL_ttf Error:\n"
                    << TTF_GetError() << ::std::endl;
        return false;
    }
    return true;
}
}  // namespace runebound::graphics