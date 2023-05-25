#ifndef RUNEBOUND_GRAPHICS_WINDOW_HPP_
#define RUNEBOUND_GRAPHICS_WINDOW_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <graphics_button.hpp>
#include <graphics_config.hpp>
#include <graphics_point.hpp>
#include <map>
#include <memory>
#include <string>

namespace runebound::graphics {
class Window {
private:
    int m_width{0};
    int m_height{0};

    bool m_is_active{false};

    SDL_Rect m_rect{};
    SDL_Color m_color{255, 255, 255, 255};

    ::std::map<::std::string, Button> m_buttons{};
    ::std::map<::std::string, Point> m_button_pos{};
    ::std::map<::std::string, bool> m_button_visible{};
    ::std::map<::std::string, bool> m_button_updatable{};

    ::std::map<::std::string, TextField> m_text_fields{};
    ::std::map<::std::string, Point> m_text_field_pos{};
    ::std::map<::std::string, TTF_Font *> m_text_field_fonts{};
    ::std::map<::std::string, SDL_Color> m_text_field_colors{};
    ::std::map<::std::string, bool> m_text_field_visible{};
    ::std::map<::std::string, bool> m_text_field_updatable{};
    ::std::string m_active_text_field{};

    ::std::map<::std::string, Texture> m_textures{};
    ::std::map<::std::string, Point> m_texture_pos{};
    ::std::map<::std::string, bool> m_texture_visible{};

    ::std::map<::std::string, ::std::unique_ptr<Window>> m_windows{};
    ::std::map<::std::string, Point> m_window_pos{};
    ::std::map<::std::string, bool> m_window_visible{};
    ::std::map<::std::string, bool> m_window_updatable{};
    ::std::string m_active_window{};

public:
    Window() = default;

    Window(SDL_Renderer *renderer, int width, int height, SDL_Color color)
        : m_width(width), m_height(height), m_color(color) {
        m_rect = {0, 0, width, height};
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
        SDL_Texture *main_texture = nullptr
    ) const;

    void render_texture(
        SDL_Renderer *renderer,
        int x_offset,
        int y_offset,
        int width,
        int height,
        SDL_Texture *texture,
        SDL_Texture *main_texture = nullptr
    ) const;

    bool handle_events(SDL_Event event);

    void update(Point mouse_pos, bool mouse_pressed);

    void add_button(
        const ::std::string &name,
        Button &button,
        Point pos,
        bool visible,
        bool updatable
    );

    void set_updatability_button(const ::std::string &name, bool state);

    void set_all_updatability_button(bool state);

    void set_visibility_button(const ::std::string &name, bool state);

    void remove_button(const ::std::string &name);

    void remove_all_buttons();

    void add_text_field(
        const ::std::string &name,
        TextField &text_field,
        TTF_Font *font,
        SDL_Color col,
        Point pos,
        bool visible,
        bool updatable
    );

    void set_updatability_text_field(const ::std::string &name, bool state);

    void set_visibility_text_field(const ::std::string &name, bool state);

    void remove_text_field(const ::std::string &name);

    void remove_all_text_fields();

    void add_texture(
        const ::std::string &name,
        Texture &texture,
        Point pos,
        bool visible
    );

    void set_visibility_texture(const ::std::string &name, bool state);

    void remove_texture(const ::std::string &name);

    void remove_all_textures();

    void add_window(
        const ::std::string &name,
        ::std::unique_ptr<Window> window,
        Point pos,
        bool visible,
        bool updatable
    );

    void set_updatability_window(const ::std::string &name, bool state);

    void set_visibility_window(const ::std::string &name, bool state);

    void remove_window(const ::std::string &name);

    void remove_all_windows();

    void activate() {
        m_is_active = true;
    }

    void deactivate() {
        m_is_active = false;
    }

    void reset_active_window();

    void set_active_window(const ::std::string &name);

    void reset_active_text_field() {
        m_active_text_field = "";
    }

    void set_active_text_field(const ::std::string &name);

    [[nodiscard]] Window *get_window(const ::std::string &name);

    [[nodiscard]] const ::std::string &get_active_window_name() const {
        return m_active_window;
    }

    [[nodiscard]] TextField *get_text_field(const ::std::string &name);

    [[nodiscard]] int width() const {
        return m_width;
    }

    [[nodiscard]] int height() const {
        return m_height;
    }

    [[nodiscard]] bool in_bounds(const Point &p) const;

    [[nodiscard]] bool is_active() const {
        return m_is_active;
    }

    void add_window_in_window(
        const std::string &win_name,
        const ::std::string &name,
        ::std::unique_ptr<Window> window,
        Point pos,
        bool visible,
        bool updatable
    );
};
}  // namespace runebound::graphics

#endif  // RUNEBOUND_GRAPHICS_WINDOW_HPP_