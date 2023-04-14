#ifndef RUNEBOUND_GRAPHICS_BUTTON_HPP_
#define RUNEBOUND_GRAPHICS_BUTTON_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <functional>
#include <graphics.hpp>
#include <graphics_point.hpp>
#include <graphics_shapes.hpp>
#include <memory>
#include <string>

namespace runebound::graphics {
class Button {
private:
    int m_x, m_y;
    int m_width, m_height;
    int m_texture_x_offset, m_texture_y_offset;
    Texture m_texture;
    SDL_Color m_fill_color, m_border_color;
    ::std::function<void()> m_on_click_function, m_on_cover_function;
    RectangleShape m_shape;

public:
    Button()
        : m_x(0),
          m_y(0),
          m_width(0),
          m_height(0),
          m_texture_x_offset(0),
          m_texture_y_offset(0),
          m_texture(),
          m_fill_color(),
          m_border_color(),
          m_on_click_function(),
          m_on_cover_function(),
          m_shape(){};

    Button(
        int x,
        int y,
        int width,
        int height,
        int texture_x_offset,
        int texture_y_offset,
        Texture &texture,
        ::std::function<void()> on_click_function,
        ::std::function<void()> on_cover_function,
        SDL_Color fill_color = {0xFF, 0xFF, 0xFF, 0xFF},
        SDL_Color border_color = {0x00, 0x00, 0x00, 0xFF}
    );

    Button(Button &&other) noexcept
        : m_x(other.m_x),
          m_y(other.m_y),
          m_width(other.m_width),
          m_height(other.m_height),
          m_texture_x_offset(other.m_texture_x_offset),
          m_texture_y_offset(other.m_texture_y_offset),
          m_texture(::std::move(other.m_texture)),
          m_fill_color(other.m_fill_color),
          m_border_color(other.m_border_color),
          m_on_click_function(::std::move(other.m_on_click_function)),
          m_on_cover_function(::std::move(other.m_on_cover_function)),
          m_shape(::std::move(other.m_shape)) {
    }

    Button &operator=(Button &&other) noexcept {
        m_x = other.m_x;
        m_y = other.m_y;
        m_width = other.m_width;
        m_height = other.m_height;
        m_texture_x_offset = other.m_texture_x_offset;
        m_texture_y_offset = other.m_texture_y_offset;
        m_texture = ::std::move(other.m_texture);
        m_fill_color = other.m_fill_color;
        m_border_color = other.m_border_color;
        m_on_click_function = ::std::move(other.m_on_click_function);
        m_on_cover_function = ::std::move(other.m_on_cover_function);
        m_shape = ::std::move(other.m_shape);
        return *this;
    }

    [[nodiscard]] bool in_bounds(const Point &p) const;

    void on_click() const {
        m_on_click_function();
    };

    void on_cover() const {
        m_on_cover_function();
    }

    void render(SDL_Renderer *renderer) const;

    void update_texture(Texture &texture) {
        m_texture.free();
        m_texture = ::std::move(texture);
    }
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_BUTTON_HPP_