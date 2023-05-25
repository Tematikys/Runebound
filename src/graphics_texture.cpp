#include <graphics_texture.hpp>
#include <iostream>

namespace runebound::graphics {
void Texture::free() {
    if (SHOW_TEXTURE_DEBUG_INFO) {
        ::std::cout << "Deallocate " << this << " with " << m_texture
                    << ::std::endl;
    }
    m_width = 0;
    m_height = 0;
    if (m_texture != nullptr) {
        SDL_DestroyTexture(m_texture);
        m_texture = nullptr;
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

void Texture::render_to_texture(
    SDL_Renderer *renderer,
    int x,
    int y,
    SDL_Texture *texture
) {
    SDL_SetRenderTarget(renderer, texture);
    const SDL_Rect renderQuad = {x, y, m_width, m_height};
    const SDL_Rect clip = {0, 0, m_width, m_height};
    SDL_RenderCopy(renderer, m_texture, &clip, &renderQuad);
    SDL_SetRenderTarget(renderer, nullptr);
}

bool Texture::load_image_from_file(
    SDL_Renderer *renderer,
    const ::std::string &path
) {
    free();
    SDL_Texture *new_texture = nullptr;
    SDL_Surface *loaded_surface = IMG_Load(path.c_str());
    if (loaded_surface == nullptr) {
        if (SHOW_TEXTURE_DEBUG_INFO) {
            ::std::cout << "Unable to load image! SDL_image Error:\n"
                        << path << ' ' << IMG_GetError() << ::std::endl;
        }
    } else {
        SDL_SetColorKey(
            loaded_surface, SDL_TRUE,
            SDL_MapRGB(loaded_surface->format, 0xFF, 0xFF, 0xFF)
        );
        new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
        if (new_texture == nullptr) {
            if (SHOW_TEXTURE_DEBUG_INFO) {
                ::std::cout << "Unable to create texture from! SDL Error:\n"
                            << path << ' ' << SDL_GetError() << ::std::endl;
            }
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
        if (SHOW_TEXTURE_DEBUG_INFO) {
            ::std::cout << "Unable to render text surface! SDL_ttf Error:\n"
                        << TTF_GetError() << ::std::endl;
        }
    } else {
        m_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        if (m_texture == nullptr) {
            if (SHOW_TEXTURE_DEBUG_INFO) {
                ::std::cout << "Unable to create texture from rendered text! "
                               "SDL Error:\n"
                            << SDL_GetError() << ::std::endl;
            }
        } else {
            m_width = text_surface->w;
            m_height = text_surface->h;
        }
        SDL_FreeSurface(text_surface);
    }
    return m_texture != nullptr;
}

Texture::Texture() : m_texture(nullptr), m_width(0), m_height(0) {
    if (SHOW_TEXTURE_DEBUG_INFO) {
        ::std::cout << "Default: " << this << ::std::endl;
    }
}

Texture::Texture(SDL_Texture *&texture) : m_texture(texture) {
    if (SHOW_TEXTURE_DEBUG_INFO) {
        ::std::cout << "Generated new texture " << this << " with " << m_texture
                    << ::std::endl;
    }
    int width{};
    int height{};
    SDL_QueryTexture(m_texture, nullptr, nullptr, &width, &height);
    texture = nullptr;
    m_width = width;
    m_height = height;
}

Texture::Texture(Texture &&other) noexcept
    : m_width(other.m_width),
      m_height(other.m_height),
      m_texture(other.m_texture) {
    if (SHOW_TEXTURE_DEBUG_INFO) {
        ::std::cout << "move() to " << this << " with " << m_texture << " from "
                    << &other << ::std::endl;
    }
    other.m_texture = nullptr;
    other.m_width = 0;
    other.m_height = 0;
    other.free();
}

Texture &Texture::operator=(Texture &&other) noexcept {
    if (SHOW_TEXTURE_DEBUG_INFO) {
        ::std::cout << "move= to " << this << " with " << m_texture << " from "
                    << &other << ::std::endl;
    }
    m_width = other.m_width;
    m_height = other.m_height;
    m_texture = other.m_texture;
    other.m_texture = nullptr;
    other.m_width = 0;
    other.m_height = 0;
    other.free();
    return *this;
}

}  // namespace runebound::graphics