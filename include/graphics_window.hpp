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
    SDL_Texture *m_texture{nullptr};
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
        : m_width(width),
          m_height(height),
          m_color(color),
          m_texture(SDL_CreateTexture(
              renderer,
              SDL_PIXELFORMAT_RGBA8888,
              SDL_TEXTUREACCESS_TARGET,
              m_width,
              m_height
          )) {
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
        SDL_Texture *main_texture = nullptr,
        double angle = 0.0,
        SDL_Point *center = nullptr,
        SDL_RendererFlip flip = SDL_FLIP_NONE
    ) const;

    void render_texture(
        SDL_Renderer *renderer,
        int x_offset,
        int y_offset,
        int width,
        int height,
        SDL_Texture *texture,
        SDL_Texture *main_texture = nullptr,
        SDL_Rect *clip = nullptr,
        double angle = 0.0,
        SDL_Point *center = nullptr,
        SDL_RendererFlip flip = SDL_FLIP_NONE
    ) const;

    void add_button(
        const ::std::string &name,
        Button &button,
        Point pos,
        bool visible,
        bool updatable
    ) {
        m_buttons[name] = ::std::move(button);
        m_button_pos[name] = pos;
        m_button_visible[name] = visible;
        m_button_updatable[name] = updatable;
    }

    void set_updatability_button(const ::std::string &name, bool state) {
        if (m_buttons.find(name) == m_buttons.end()) {
            return;
        }
        m_button_updatable[name] = state;
    }

    void set_visibility_button(const ::std::string &name, bool state) {
        if (m_buttons.find(name) == m_buttons.end()) {
            return;
        }
        m_button_visible[name] = state;
    }

    void remove_button(const ::std::string &name) {
        if (m_buttons.find(name) == m_buttons.end()) {
            return;
        }
        m_buttons.erase(name);
        m_button_pos.erase(name);
        m_button_visible.erase(name);
        m_button_updatable.erase(name);
    }

    void remove_all_buttons() {
        m_buttons.clear();
        m_button_pos.clear();
        m_button_visible.clear();
        m_button_updatable.clear();
    }

    void add_text_field(
        const ::std::string &name,
        TextField &text_field,
        TTF_Font *font,
        SDL_Color col,
        Point pos,
        bool visible,
        bool updatable
    ) {
        m_text_fields[name] = ::std::move(text_field);
        m_text_field_fonts[name] = font;
        m_text_field_colors[name] = col;
        m_text_field_pos[name] = pos;
        m_text_field_visible[name] = visible;
        m_text_field_updatable[name] = updatable;
    }

    void set_updatability_text_field(const ::std::string &name, bool state) {
        if (m_text_fields.find(name) == m_text_fields.end()) {
            return;
        }
        m_text_field_updatable[name] = state;
    }

    void set_visibility_text_field(const ::std::string &name, bool state) {
        if (m_text_fields.find(name) == m_text_fields.end()) {
            return;
        }
        m_text_field_visible[name] = state;
    }

    void remove_text_field(const ::std::string &name) {
        if (m_text_fields.find(name) == m_text_fields.end()) {
            return;
        }
        m_text_fields.erase(name);
        m_text_field_pos.erase(name);
        m_text_field_visible.erase(name);
        m_text_field_updatable.erase(name);
    }

    void remove_all_text_fields() {
        m_text_fields.clear();
        m_text_field_pos.clear();
        m_text_field_visible.clear();
        m_text_field_updatable.clear();
    }

    void add_texture(
        const ::std::string &name,
        Texture &texture,
        Point pos,
        bool visible
    ) {
        m_textures[name] = ::std::move(texture);
        m_texture_pos[name] = pos;
        m_texture_visible[name] = visible;
    }

    void set_visibility_texture(const ::std::string &name, bool state) {
        if (m_textures.find(name) == m_textures.end()) {
            return;
        }
        m_texture_visible[name] = state;
    }

    void remove_texture(const ::std::string &name) {
        if (m_textures.find(name) == m_textures.end()) {
            return;
        }
        m_textures.erase(name);
        m_texture_pos.erase(name);
        m_texture_visible.erase(name);
    }

    void remove_all_textures() {
        m_textures.clear();
        m_texture_pos.clear();
        m_texture_visible.clear();
    }

    void add_window(
        const ::std::string &name,
        ::std::unique_ptr<Window> window,
        Point pos,
        bool visible,
        bool updatable
    ) {
        m_windows[name] = ::std::move(window);
        m_window_pos[name] = pos;
        m_window_visible[name] = visible;
        m_window_updatable[name] = updatable;
    }

    void set_updatability_window(const ::std::string &name, bool state) {
        if (m_windows.find(name) == m_windows.end()) {
            return;
        }
        m_window_updatable[name] = state;
    }

    void set_visibility_window(const ::std::string &name, bool state) {
        if (m_windows.find(name) == m_windows.end()) {
            return;
        }
        m_window_visible[name] = state;
    }

    void remove_window(const ::std::string &name) {
        if (m_windows.find(name) == m_windows.end()) {
            return;
        }
        m_windows.erase(name);
        m_window_pos.erase(name);
        m_window_visible.erase(name);
        m_window_updatable.erase(name);
    }

    void remove_all_windows() {
        m_windows.clear();
        m_window_pos.clear();
        m_window_visible.clear();
        m_window_updatable.clear();
    }

    void handle_events(SDL_Event event);

    void update(Point mouse_pos, bool mouse_pressed);

    void activate() {
        m_is_active = true;
    }

    void deactivate() {
        m_is_active = false;
    }

    void reset_active_window() {
        if (!m_active_window.empty()) {
            m_windows[m_active_window]->deactivate();
        }
    }

    void set_active_window(const ::std::string &name) {
        if (m_windows.find(name) == m_windows.end()) {
            return;
        }
        reset_active_window();
        m_active_window = name;
        m_windows[name]->activate();
    }

    void reset_active_text_field() {
        m_active_text_field = "";
    }

    void set_active_text_field(const ::std::string &name) {
        if (m_text_fields.find(name) == m_text_fields.end()) {
            return;
        }
        reset_active_text_field();
        m_active_text_field = name;
    }

    [[nodiscard]] SDL_Texture *get_texture() {
        return m_texture;
    }

    [[nodiscard]] Window *get_window(const ::std::string &name) {
        if (m_windows.find(name) != m_windows.end()) {
            return m_windows[name].get();
        }
        return nullptr;
    }

    [[nodiscard]] const ::std::string &get_active_window_name() const {
        return m_active_window;
    }

    [[nodiscard]] TextField *get_text_field(const ::std::string &name) {
        if (m_text_fields.find(name) == m_text_fields.end()) {
            return nullptr;
        }
        return &m_text_fields.at(name);
    }

    [[nodiscard]] bool in_bounds(const Point &p) const {
        return (0 <= p.x() && p.x() < m_width) &&
               (0 <= p.y() && p.y() < m_height);
    }

    [[nodiscard]] bool is_active() const {
        return m_is_active;
    }
};
}  // namespace runebound::graphics

#endif  // RUNEBOUND_GRAPHICS_WINDOW_HPP_