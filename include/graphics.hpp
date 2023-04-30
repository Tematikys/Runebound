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
    SDL_Texture *m_texture{nullptr};
    int m_x = 0, m_y = 0;
    int m_width{0}, m_height{0};

public:
    Texture() = default;

    Texture(Texture &&other) noexcept
        : m_width(other.m_width),
          m_height(other.m_height),
          m_x(other.m_x),
          m_y(other.m_y),
          m_texture(other.m_texture) {
        other.m_texture = nullptr;
    }

    Texture(const Texture &other) = delete;

    Texture &operator=(Texture &&other) noexcept {
        m_width = other.m_width;
        m_height = other.m_height;
        m_x = other.m_x;
        m_y = other.m_y;
        m_texture = other.m_texture;
        other.m_texture = nullptr;
        return *this;
    }

    Texture &operator=(const Texture &other) = delete;

    ~Texture() {
        free();
    };

    bool
    load_image_from_file(SDL_Renderer *renderer, const ::std::string &path);

    bool load_text_from_string(
        SDL_Renderer *renderer,
        TTF_Font *font,
        const ::std::string &text,
        SDL_Color color
    );

    void free();

    void render(
        SDL_Renderer *renderer,
        int x,
        int y,
        SDL_Rect *clip = nullptr,
        double angle = 0.0,
        SDL_Point *center = nullptr,
        SDL_RendererFlip flip = SDL_FLIP_NONE
    ) const;

    [[nodiscard]] int width() const {
        return m_width;
    };

    [[nodiscard]] int height() const {
        return m_height;
    };
};

Point get_center_of_hexagon(int i, int j);

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