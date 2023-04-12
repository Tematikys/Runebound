#ifndef RUNEBOUND_GRAPHICS_BUTTON_HPP_
#define RUNEBOUND_GRAPHICS_BUTTON_HPP_

#include <functional>
#include <graphics.hpp>
#include <graphics_point.hpp>
#include <graphics_shapes.hpp>
#include <memory>

namespace runebound::graphics {
class RectButton {
private:
    int m_x, m_y, m_width, m_height, m_texture_x_offset, m_texture_y_offset;
    Texture m_texture;
    SDL_Color m_fill_color, m_border_color;
    ::std::function<void()> m_on_click_function, m_on_cover_function;
    RectangleShape m_shape;

public:
    RectButton(
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

    [[nodiscard]] bool in_bounds(const Point &p) const;

    void on_click() const {
        m_on_click_function();
    };

    void on_cover() const {
        m_on_cover_function();
    }

    void render(SDL_Renderer *renderer, int x_offset = 0, int y_offset = 0)
        const;
};

class CircButton {
private:
    int m_x, m_y, m_width, m_height, m_radius, m_texture_x_offset,
        m_texture_y_offset;
    Point m_center;
    Texture m_texture;
    SDL_Color m_fill_color, m_border_color;
    ::std::function<void()> m_on_click_function, m_on_cover_function;
    CircleShape m_shape;

public:
    CircButton(
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

    [[nodiscard]] bool in_bounds(const Point &p) const;

    void on_click() const {
        m_on_click_function();
    };

    void on_cover() const {
        m_on_cover_function();
    }

    void render(SDL_Renderer *renderer, int x_offset = 0, int y_offset = 0)
        const;
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_BUTTON_HPP_