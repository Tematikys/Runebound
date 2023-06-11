#include <graphics_window.hpp>
#include <iostream>

namespace runebound::graphics {
Window::Window(Window &&other) noexcept
    : m_width(other.m_width),
      m_height(other.m_height),
      m_rect(other.m_rect),
      m_buttons(std::move(other.m_buttons)),
      m_button_pos(std::move(other.m_button_pos)),
      m_text_fields(std::move(other.m_text_fields)),
      m_text_field_pos(std::move(other.m_text_field_pos)),
      m_text_field_fonts(std::move(other.m_text_field_fonts)),
      m_text_field_colors(std::move(other.m_text_field_colors)),
      m_textures(std::move(other.m_textures)),
      m_texture_pos(std::move(other.m_texture_pos)),
      m_windows(std::move(other.m_windows)),
      m_window_pos(std::move(other.m_window_pos)) {
}

Window &Window::operator=(Window &&other) noexcept {
    m_width = other.m_width;
    m_height = other.m_height;
    m_rect = other.m_rect;
    m_buttons = std::move(other.m_buttons);
    m_button_pos = std::move(other.m_button_pos);
    m_text_fields = std::move(other.m_text_fields);
    m_text_field_pos = std::move(other.m_text_field_pos);
    m_text_field_fonts = std::move(other.m_text_field_fonts);
    m_text_field_colors = std::move(other.m_text_field_colors);
    m_textures = std::move(other.m_textures);
    m_texture_pos = std::move(other.m_texture_pos);
    m_windows = std::move(other.m_windows);
    m_window_pos = std::move(other.m_window_pos);
    return *this;
}

void Window::render(
    SDL_Renderer *renderer,
    int x_offset,
    int y_offset,
    SDL_Texture *main_texture
) const {
    SDL_Texture *tex = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_width,
        m_height
    );
    SDL_SetRenderTarget(renderer, tex);

    SDL_SetRenderDrawColor(
        renderer, m_color.r, m_color.g, m_color.b, m_color.a
    );
    SDL_RenderClear(renderer);

    for (const auto &[name, texture] : m_textures) {
        if (m_texture_visible.at(name)) {
            texture.render(
                renderer, m_texture_pos.at(name).x(), m_texture_pos.at(name).y()
            );
        }
    }

    for (const auto &[name, button] : m_buttons) {
        if (m_button_visible.at(name)) {
            button.render(
                renderer, m_button_pos.at(name).x(), m_button_pos.at(name).y()
            );
        }
    }

    for (const auto &[name, text_field] : m_text_fields) {
        if (m_text_field_visible.at(name)) {
            text_field.render(
                renderer, m_text_field_fonts.at(name),
                m_text_field_colors.at(name), m_text_field_pos.at(name).x(),
                m_text_field_pos.at(name).y()
            );
        }
    }

    for (const auto &[name, window] : m_windows) {
        if (m_window_visible.at(name)) {
            window->render(
                renderer, m_window_pos.at(name).x(), m_window_pos.at(name).y(),
                tex
            );
        }
    }

    const RectangleShape rect{0, 0, m_width - 1, m_height - 1};
    rect.render_border(renderer, 0, 0, {255, 0, 0, 255}, 1);

    SDL_SetRenderTarget(renderer, main_texture);
    const SDL_Rect renderQuad = {x_offset, y_offset, m_width, m_height};
    SDL_RenderCopy(renderer, tex, &m_rect, &renderQuad);
    SDL_DestroyTexture(tex);
}

void Window::render_texture(
    SDL_Renderer *renderer,
    int x_offset,
    int y_offset,
    int width,
    int height,
    SDL_Texture *texture,
    SDL_Texture *main_texture
) const {
    SDL_Texture *tex = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_width,
        m_height
    );
    SDL_SetRenderTarget(renderer, tex);
    const SDL_Rect renderQuad = {x_offset, y_offset, width, height};
    SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);
    SDL_SetRenderTarget(renderer, main_texture);
    SDL_DestroyTexture(tex);
}

bool Window::handle_events(SDL_Event event) {
    if (!m_is_active) {
        return false;
    }
    bool is_updated = false;
    if (!m_active_window.empty()) {
        is_updated |= m_windows[m_active_window]->handle_events(event);
    }
    switch (event.type) {
        case SDL_TEXTINPUT:
            if (!m_active_text_field.empty() &&
                (((SDL_GetModState() & KMOD_CTRL) == 0) ||
                 (event.text.text[0] != 'c' && event.text.text[0] != 'C' &&
                  event.text.text[0] != 'v' && event.text.text[0] != 'V'))) {
                m_text_fields[m_active_text_field].push(event.text.text);
                is_updated = true;
            }
            break;
        case SDL_KEYDOWN:
            if (m_active_text_field.empty()) {
                break;
            }
            if (event.key.keysym.sym == SDLK_BACKSPACE) {
                m_text_fields[m_active_text_field].pop();
                is_updated = true;
            } else if (event.key.keysym.sym == SDLK_c && ((SDL_GetModState() & KMOD_CTRL) != 0)) {
                SDL_SetClipboardText(
                    m_text_fields[m_active_text_field].get().c_str()
                );
                is_updated = true;
            } else if (event.key.keysym.sym == SDLK_v && ((SDL_GetModState() & KMOD_CTRL) != 0)) {
                m_text_fields[m_active_text_field].clear();
                m_text_fields[m_active_text_field].push(SDL_GetClipboardText());
                is_updated = true;
            }
            break;
    }
    return is_updated;
}

bool Window::update(Point mouse_pos, bool &mouse_pressed) {
    if (!m_is_active) {
        return false;
    }
    bool updated = false;
    if (!m_active_window.empty()) {
        updated |= m_windows[m_active_window]->update(mouse_pos, mouse_pressed);
    }
    for (const auto &[name, window] : m_windows) {
        if (m_window_updatable[name] &&
            window->in_bounds(
                mouse_pos -
                Point(m_window_pos[name].x(), m_window_pos[name].y())
            ) &&
            mouse_pressed) {
            set_active_window(name);
            updated |= window->update(
                mouse_pos -
                    Point(m_window_pos[name].x(), m_window_pos[name].y()),
                mouse_pressed
            );
            mouse_pressed = false;
            break;
        }
    }
    for (const auto &[name, text_field] : m_text_fields) {
        if (m_text_field_updatable[name] &&
            text_field.in_bounds(
                mouse_pos -
                Point(m_text_field_pos[name].x(), m_text_field_pos[name].y())
            )) {
            text_field.on_cover();
            if (mouse_pressed) {
                mouse_pressed = false;
                text_field.on_click();
                updated = true;
            }
        }
    }
    if (mouse_pressed) {
        reset_active_window();
        reset_active_text_field();
        updated = true;
    }
    for (const auto &[name, button] : m_buttons) {
        if (m_button_updatable[name] &&
            button.in_bounds(
                mouse_pos -
                Point(m_button_pos[name].x(), m_button_pos[name].y())
            )) {
            button.on_cover();
            if (mouse_pressed) {
                mouse_pressed = false;
                button.on_click();
                updated = true;
            }
        }
    }
    return updated;
}

void Window::add_button(
    const std::string &name,
    Button &button,
    Point pos,
    bool visible,
    bool updatable
) {
    m_buttons[name] = std::move(button);
    m_button_pos[name] = pos;
    m_button_visible[name] = visible;
    m_button_updatable[name] = updatable;
}

void Window::set_updatability_button(const std::string &name, bool state) {
    if (m_buttons.find(name) == m_buttons.end()) {
        return;
    }
    m_button_updatable[name] = state;
}

void Window::set_all_updatability_button(bool state) {
    for (const auto &[name, button] : m_buttons) {
        set_updatability_button(name, state);
    }
}

void Window::set_visibility_button(const std::string &name, bool state) {
    if (m_buttons.find(name) == m_buttons.end()) {
        return;
    }
    m_button_visible[name] = state;
}

void Window::remove_button(const std::string &name) {
    if (m_buttons.find(name) == m_buttons.end()) {
        return;
    }
    m_buttons.erase(name);
    m_button_pos.erase(name);
    m_button_visible.erase(name);
    m_button_updatable.erase(name);
}

void Window::remove_all_buttons() {
    m_buttons.clear();
    m_button_pos.clear();
    m_button_visible.clear();
    m_button_updatable.clear();
}

void Window::add_text_field(
    const std::string &name,
    TextField &text_field,
    TTF_Font *font,
    SDL_Color col,
    Point pos,
    bool visible,
    bool updatable
) {
    m_text_fields[name] = std::move(text_field);
    m_text_field_fonts[name] = font;
    m_text_field_colors[name] = col;
    m_text_field_pos[name] = pos;
    m_text_field_visible[name] = visible;
    m_text_field_updatable[name] = updatable;
}

void Window::set_updatability_text_field(const std::string &name, bool state) {
    if (m_text_fields.find(name) == m_text_fields.end()) {
        return;
    }
    m_text_field_updatable[name] = state;
}

void Window::set_visibility_text_field(const std::string &name, bool state) {
    if (m_text_fields.find(name) == m_text_fields.end()) {
        return;
    }
    m_text_field_visible[name] = state;
}

void Window::remove_text_field(const std::string &name) {
    if (m_text_fields.find(name) == m_text_fields.end()) {
        return;
    }
    m_text_fields.erase(name);
    m_text_field_pos.erase(name);
    m_text_field_visible.erase(name);
    m_text_field_updatable.erase(name);
}

void Window::remove_all_text_fields() {
    m_text_fields.clear();
    m_text_field_pos.clear();
    m_text_field_visible.clear();
    m_text_field_updatable.clear();
}

void Window::add_texture(
    const std::string &name,
    Texture &texture,
    Point pos,
    bool visible
) {
    if (m_textures.find(name) != m_textures.end()) {
        m_textures[name].free();
    }
    m_textures[name] = std::move(texture);
    m_texture_pos[name] = pos;
    m_texture_visible[name] = visible;
}

void Window::set_visibility_texture(const std::string &name, bool state) {
    if (m_textures.find(name) == m_textures.end()) {
        return;
    }
    m_texture_visible[name] = state;
}

void Window::remove_texture(const std::string &name) {
    if (m_textures.find(name) == m_textures.end()) {
        return;
    }
    m_textures[name].free();
    m_textures.erase(name);
    m_texture_pos.erase(name);
    m_texture_visible.erase(name);
}

void Window::remove_all_textures() {
    m_textures.clear();
    m_texture_pos.clear();
    m_texture_visible.clear();
}

void Window::add_window(
    const std::string &name,
    std::unique_ptr<Window> window,
    Point pos,
    bool visible,
    bool updatable
) {
    m_windows[name] = std::move(window);
    m_window_pos[name] = pos;
    m_window_visible[name] = visible;
    m_window_updatable[name] = updatable;
}

void Window::set_updatability_window(const std::string &name, bool state) {
    if (m_windows.find(name) == m_windows.end()) {
        return;
    }
    m_window_updatable[name] = state;
}

void Window::set_visibility_window(const std::string &name, bool state) {
    if (m_windows.find(name) == m_windows.end()) {
        return;
    }
    m_window_visible[name] = state;
}

void Window::remove_window(const std::string &name) {
    if (m_windows.find(name) == m_windows.end()) {
        return;
    }
    m_windows.erase(name);
    m_window_pos.erase(name);
    m_window_visible.erase(name);
    m_window_updatable.erase(name);
}

void Window::remove_all_windows() {
    m_windows.clear();
    m_window_pos.clear();
    m_window_visible.clear();
    m_window_updatable.clear();
}

void Window::reset_active_window() {
    if (!m_active_window.empty()) {
        m_windows[m_active_window]->deactivate();
        m_active_window = "";
    }
}

void Window::set_active_window(const std::string &name) {
    if (m_windows.find(name) == m_windows.end()) {
        return;
    }
    reset_active_window();
    m_active_window = name;
    m_windows[name]->activate();
}

void Window::set_active_text_field(const std::string &name) {
    if (m_text_fields.find(name) == m_text_fields.end()) {
        return;
    }
    reset_active_text_field();
    m_active_text_field = name;
}

Window *Window::get_window(const std::string &name) {
    if (m_windows.find(name) != m_windows.end()) {
        return m_windows[name].get();
    }
    return nullptr;
}

void Window::add_window_in_window(
    const std::string &win_name,
    const std::string &name,
    std::unique_ptr<Window> window,
    Point pos,
    bool visible,
    bool updatable
) {
    if (m_windows.find(win_name) != m_windows.end()) {
        m_windows[win_name]->add_window(
            name, std::move(window), pos, visible, updatable
        );
    }
}

TextField *Window::get_text_field(const std::string &name) {
    if (m_text_fields.find(name) == m_text_fields.end()) {
        return nullptr;
    }
    return &m_text_fields.at(name);
}

bool Window::in_bounds(const Point &p) const {
    return (0 <= p.x() && p.x() < m_width) && (0 <= p.y() && p.y() < m_height);
}

}  // namespace runebound::graphics