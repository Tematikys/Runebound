#ifndef RUNEBOUND_GRAPHICS_WINDOW_HPP_
#define RUNEBOUND_GRAPHICS_WINDOW_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <graphics_button.hpp>
#include <graphics_config.hpp>
#include <graphics_point.hpp>
#include <vector>

namespace runebound::graphics {
class Window {
private:
    int m_width{0};
    int m_height{0};

    SDL_Rect m_rect{};
    SDL_Texture *m_texture{nullptr};
    SDL_Color m_color{255, 255, 255, 255};

    ::std::vector<Button> m_buttons{};
    ::std::vector<Point> m_button_pos{};

    ::std::vector<TextField> m_text_fields{};
    ::std::vector<Point> m_text_field_pos{};
    ::std::vector<TTF_Font *> m_text_field_fonts{};
    ::std::vector<SDL_Color> m_text_field_colors{};
    ::std::size_t m_active_text_filed{0};

    ::std::vector<Texture> m_textures{};
    ::std::vector<Point> m_texture_pos{};

    ::std::vector<Window *> m_windows{};
    ::std::vector<Point> m_window_pos{};
    ::std::size_t m_active_window{0};

public:
    Window() = default;

    Window(SDL_Renderer *renderer, int width, int height, SDL_Color color)
        : m_width(width), m_height(height), m_color(color) {
        m_rect = {0, 0, width, height};
        m_texture = SDL_CreateTexture(
            renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
            m_width, m_height
        );
    }

    Window(const Window &other) = delete;

    Window(Window &&other) noexcept;

    Window &operator=(const Window &other) = delete;

    Window &operator=(Window &&other) noexcept;

    ~Window() = default;

    void render(
        SDL_Renderer *renderer,
        int x_offset,
        int y_offset,
        SDL_Texture *main_texture = nullptr,
        double angle = 0.0,
        SDL_Point *center = nullptr,
        SDL_RendererFlip flip = SDL_FLIP_NONE
    ) const;

    void click(Point mouse_pos);

    void add_button(Button &button, Point pos) {
        m_buttons.push_back(::std::move(button));
        m_button_pos.push_back(pos);
    }

    void add_text_files(
        TextField &text_field,
        TTF_Font *font,
        SDL_Color col,
        Point pos
    ) {
        m_text_fields.push_back(::std::move(text_field));
        m_text_field_pos.push_back(pos);
        m_text_field_fonts.push_back(font);
        m_text_field_colors.push_back(col);
    }

    void add_texture(Texture &texture, Point pos) {
        m_textures.push_back(::std::move(texture));
        m_texture_pos.push_back(pos);
    }

    void add_window(Window *&window, Point pos) {
        m_windows.push_back(::std::move(window));
        m_window_pos.push_back(pos);
    }

    [[nodiscard]] bool in_bounds(const Point &p) const {
        return (0 <= p.x() && p.x() < m_width) &&
               (0 <= p.y() && p.y() < m_height);
    }
};
}  // namespace runebound::graphics

#endif  // RUNEBOUND_GRAPHICS_WINDOW_HPP_