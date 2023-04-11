#ifndef RUNEBOUND_GRAPHICS_BUTTON_HPP_
#define RUNEBOUND_GRAPHICS_BUTTON_HPP_

#include <functional>
#include <graphics.hpp>
#include <graphics_point.hpp>
#include <graphics_shapes.hpp>
#include <memory>

namespace runebound::graphics {
class Button {
public:
    Button() = default;
    [[nodiscard]] virtual bool in_bounds(const Point &p) const = 0;
    virtual void on_click() const = 0;
    virtual void render(SDL_Renderer *renderer) const = 0;
    virtual ~Button() = default;
};

class RectButton : public Button {
private:
    int m_x, m_y, m_width, m_height;
    Texture m_texture;
    SDL_Color m_fill_color, m_border_color;
    ::std::function<void()> m_function;
    RectangleShape m_shape;

public:
    RectButton(
        int x,
        int y,
        Texture &tex,
        ::std::function<void()> func,
        SDL_Color fill_color = {0xFF, 0xFF, 0xFF, 0xFF},
        SDL_Color border_color = {0x00, 0x00, 0x00, 0xFF}
    );

    [[nodiscard]] bool in_bounds(const Point &p) const final;

    void on_click() const final {
        m_function();
    };

    void render(SDL_Renderer *renderer) const final {
        m_shape.render(renderer, m_fill_color, m_border_color);
        m_texture.render(renderer, m_x, m_y);
    };
};

class CircButton : public Button {
private:
    int m_x, m_y, m_width, m_height, m_radius;
    Point m_center;
    Texture m_texture;
    SDL_Color m_fill_color, m_border_color;
    ::std::function<void()> m_function;
    CircleShape m_shape;

public:
    CircButton(
        int x,
        int y,
        Texture &tex,
        ::std::function<void()> func,
        SDL_Color fill_color = {0xFF, 0xFF, 0xFF, 0xFF},
        SDL_Color border_color = {0x00, 0x00, 0x00, 0xFF}
    );

    [[nodiscard]] bool in_bounds(const Point &p) const final;

    void on_click() const final {
        m_function();
    };

    void render(SDL_Renderer *renderer) const final {
        m_shape.render(renderer, m_fill_color, m_border_color);
        m_texture.render(renderer, m_x, m_y);
    };
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_BUTTON_HPP_