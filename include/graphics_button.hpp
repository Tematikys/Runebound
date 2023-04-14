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
class RectButton {
private:
    int m_x, m_y;
    int m_width, m_height;
    int m_texture_x_offset, m_texture_y_offset;
    Texture m_texture;
    SDL_Color m_fill_color, m_border_color;
    ::std::function<void()> m_on_click_function, m_on_cover_function;
    RectangleShape m_shape;

public:
    RectButton()
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

    RectButton(RectButton &&other) noexcept
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

    RectButton &operator=(RectButton &&other) noexcept {
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

    void render(SDL_Renderer *renderer) const;
};

class InputHandler {
private:
    ::std::string m_text;
    bool m_active;
    bool m_update;

public:
    InputHandler() : m_text(), m_active(false), m_update(false){};

    InputHandler(InputHandler &&other) noexcept
        : m_text(::std::move(other.m_text)),
          m_active(other.m_active),
          m_update(other.m_update){};

    InputHandler &operator=(InputHandler &&other) noexcept {
        m_text = ::std::move(other.m_text);
        m_active = other.m_active;
        m_update = other.m_update;
        return *this;
    }

    void activate() {
        SDL_StartTextInput();
        m_active = true;
    }

    void deactivate() {
        SDL_StopTextInput();
        m_active = false;
    }

    [[nodiscard]] ::std::string get_text() const {
        return m_text;
    }

    [[nodiscard]] bool is_update() const {
        return m_update;
    }

    void update() {
        m_update = false;
        if (!m_active) {
            return;
        }
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_TEXTINPUT:
                    if (!(SDL_GetModState() & KMOD_CTRL &&
                          (e.text.text[0] == 'c' || e.text.text[0] == 'C' ||
                           e.text.text[0] == 'v' || e.text.text[0] == 'V'))) {
                        m_text += e.text.text;
                        m_update = true;
                    }
                    break;
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == SDLK_BACKSPACE &&
                        m_text.length() > 0) {
                        m_text.pop_back();
                        m_update = true;
                    } else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
                        SDL_SetClipboardText(m_text.c_str());
                    } else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
                        m_text = SDL_GetClipboardText();
                        m_update = true;
                    }
                    break;
            }
        }
    }
};

class TextButton {
private:
    RectButton m_button;
    InputHandler m_handler;
    TTF_Font *m_font;
    SDL_Color m_color;

public:
    TextButton() : m_button(), m_handler(), m_font(nullptr), m_color(){};

    TextButton(RectButton &button, TTF_Font *font, SDL_Color color)
        : m_button(::std::move(button)), m_font(font), m_color(color){};

    TextButton(TextButton &&other) noexcept
        : m_button(::std::move(other.m_button)),
          m_handler(::std::move(other.m_handler)),
          m_font(other.m_font),
          m_color(other.m_color) {
        other.m_font = nullptr;
    }

    TextButton &operator=(TextButton &&other) noexcept {
        m_button = ::std::move(other.m_button);
        m_handler = ::std::move(m_handler);
        m_font = other.m_font;
        other.m_font = nullptr;
        m_color = other.m_color;
        return *this;
    }

    [[nodiscard]] bool in_bounds(const Point &p) const {
        return m_button.in_bounds(p);
    };

    void on_click() {
        m_handler.activate();
        m_button.on_click();
    };

    void deactivate() {
        m_handler.deactivate();
    }

    void on_cover() {
        m_button.on_cover();
    }

    void update() {
        m_handler.update();
    }

    void render(SDL_Renderer *renderer) {
        m_handler.update();
        if (m_handler.is_update()) {
            Texture new_texture;
            new_texture.load_from_string(
                renderer, m_font, m_handler.get_text(), m_color
            );
            m_button.update_texture(new_texture);
        }
        m_button.render(renderer);
    }
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_BUTTON_HPP_