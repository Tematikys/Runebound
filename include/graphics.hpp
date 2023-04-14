#ifndef RUNEBOUND_GRAPHICS_HPP_
#define RUNEBOUND_GRAPHICS_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <graphics_config.hpp>
#include <graphics_point.hpp>
#include <string>

namespace runebound::graphics {
class Texture {
private:
    SDL_Texture *m_texture;
    int m_x, m_y;
    int m_width, m_height;

public:
    Texture() : m_texture(nullptr), m_x(0), m_y(0), m_width(0), m_height(0){};

    Texture(Texture &&other) noexcept
        : m_width(other.m_width),
          m_height(other.m_height),
          m_x(other.m_x),
          m_y(other.m_y),
          m_texture(other.m_texture) {
        other.m_texture = nullptr;
    }

    Texture &operator=(Texture &&other) noexcept {
        m_width = other.m_width;
        m_height = other.m_height;
        m_x = other.m_x;
        m_y = other.m_y;
        m_texture = other.m_texture;
        other.m_texture = nullptr;
        return *this;
    }

    ~Texture() {
        free();
    };

    void set_coords(int x, int y) {
        m_x = x;
        m_y = y;
    }

    bool
    load_image_from_file(SDL_Renderer *renderer, const ::std::string &path);

    bool load_from_string(
        SDL_Renderer *renderer,
        TTF_Font *font,
        const ::std::string &text,
        SDL_Color color
    );

    void free();

    void set_color(uint8_t red, uint8_t green, uint8_t blue) {
        SDL_SetTextureColorMod(m_texture, red, green, blue);
    };

    void set_blend_mode(SDL_BlendMode blending) {
        SDL_SetTextureBlendMode(m_texture, blending);
    };

    void set_alpha(uint8_t alpha) {
        SDL_SetTextureAlphaMod(m_texture, alpha);
    };

    void render(
        SDL_Renderer *renderer,
        int x_off,
        int y_off,
        SDL_Rect *clip = nullptr,
        double angle = 0.0,
        SDL_Point *center = nullptr,
        SDL_RendererFlip flip = SDL_FLIP_NONE
    ) const;

    [[nodiscard]] int get_width() const {
        return m_width;
    };

    [[nodiscard]] int get_height() const {
        return m_height;
    };
};

// SDL init function
bool SDL_init(SDL_Window *&window, SDL_Renderer *&renderer);

void update_mouse_pos(Point &pos);

bool generate_text(
    SDL_Renderer *renderer,
    Texture &texture,
    const ::std::string &text,
    TTF_Font *font,
    SDL_Color color
);

bool load_font(TTF_Font *&font, const ::std::string &path, int font_size);
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_HPP_