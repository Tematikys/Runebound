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
    int m_x{0};
    int m_y{0};
    int m_width{0};
    int m_height{0};
    int m_texture_x_offset{0};
    int m_texture_y_offset{0};
    Texture m_texture{};
    SDL_Color m_fill_color{};
    SDL_Color m_border_color{};
    ::std::function<void()> m_on_click_function{};
    ::std::function<void()> m_on_cover_function{};
    RectangleShape m_shape{};

public:
    Button() = default;

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

    Button(Button &&other) noexcept;

    Button(const Button &other) = delete;

    Button &operator=(Button &&other) noexcept;

    Button &operator=(const Button &other) = delete;

    ~Button() = default;

    void on_click() const {
        m_on_click_function();
    };

    void on_cover() const {
        m_on_cover_function();
    }

    void render(SDL_Renderer *renderer) const;

    [[nodiscard]] bool in_bounds(const Point &p) const;

    [[nodiscard]] Point get_coords() const {
        return {m_x, m_y};
    }
};

class TextField {
private:
    ::std::string m_text{};
    int m_max_text_len{0};
    Button m_button{};

public:
    TextField() = default;

    TextField(::std::string text, Button &button, int max_len = 0);

    void push(const ::std::string &suffix);

    void pop() {
        if (!m_text.empty()) {
            m_text.pop_back();
        }
    }

    void clear() {
        m_text = "";
    }

    void on_click() const {
        m_button.on_click();
    }

    void on_cover() const {
        m_button.on_cover();
    }

    void render(
        SDL_Renderer *renderer,
        TTF_Font *font,
        SDL_Color color,
        SDL_Rect *clip = nullptr,
        double angle = 0.0,
        SDL_Point *center = nullptr,
        SDL_RendererFlip flip = SDL_FLIP_NONE
    ) const;

    [[nodiscard]] bool in_bounds(const Point &p) const {
        return m_button.in_bounds(p);
    }

    [[nodiscard]] const ::std::string &get() const {
        return m_text;
    }
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_BUTTON_HPP_