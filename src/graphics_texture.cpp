#include <graphics_texture.hpp>
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
}  // namespace runebound::graphics