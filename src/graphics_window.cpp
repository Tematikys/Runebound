#include <graphics_window.hpp>
#include <iostream>

namespace runebound::graphics {
Window::Window(Window &&other) noexcept
    : m_width(other.m_width),
      m_height(other.m_height),
      m_rect(other.m_rect),
      m_texture(other.m_texture),
      m_buttons(::std::move(other.m_buttons)),
      m_button_pos(::std::move(other.m_button_pos)),
      m_text_fields(::std::move(other.m_text_fields)),
      m_text_field_pos(::std::move(other.m_text_field_pos)),
      m_text_field_fonts(::std::move(other.m_text_field_fonts)),
      m_text_field_colors(::std::move(other.m_text_field_colors)),
      m_textures(::std::move(other.m_textures)),
      m_texture_pos(::std::move(other.m_texture_pos)),
      m_windows(::std::move(other.m_windows)),
      m_window_pos(::std::move(other.m_window_pos)) {
    other.m_texture = nullptr;
}

Window &Window::operator=(Window &&other) noexcept {
    m_width = other.m_width;
    m_height = other.m_height;
    m_rect = other.m_rect;
    m_texture = other.m_texture;
    other.m_texture = nullptr;
    m_buttons = ::std::move(other.m_buttons);
    m_button_pos = ::std::move(other.m_button_pos);
    m_text_fields = ::std::move(other.m_text_fields);
    m_text_field_pos = ::std::move(other.m_text_field_pos);
    m_text_field_fonts = ::std::move(other.m_text_field_fonts);
    m_text_field_colors = ::std::move(other.m_text_field_colors);
    m_textures = ::std::move(other.m_textures);
    m_texture_pos = ::std::move(other.m_texture_pos);
    m_windows = ::std::move(other.m_windows);
    m_window_pos = ::std::move(other.m_window_pos);
    return *this;
}

void Window::render(
    SDL_Renderer *renderer,
    int x_offset,
    int y_offset,
    SDL_Texture *main_texture,
    double angle,
    SDL_Point *center,
    SDL_RendererFlip flip
) const {
    SDL_SetRenderTarget(renderer, m_texture);

    SDL_SetRenderDrawColor(
        renderer, m_color.r, m_color.g, m_color.b, m_color.a
    );
    SDL_RenderClear(renderer);

    const RectangleShape rect{0, 0, m_width - 1, m_height - 1};
    rect.render_border(renderer, 0, 0, {255, 0, 0, 255});

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

    for (const auto &[name, texture] : m_textures) {
        if (m_texture_visible.at(name)) {
            texture.render(
                renderer, m_texture_pos.at(name).x(), m_texture_pos.at(name).y()
            );
        }
    }

    for (const auto &[name, window] : m_windows) {
        if (m_window_visible.at(name)) {
            window->render(
                renderer, m_window_pos.at(name).x(), m_window_pos.at(name).y(),
                m_texture
            );
        }
    }
    SDL_SetRenderTarget(renderer, main_texture);
    const SDL_Rect renderQuad = {x_offset, y_offset, m_width, m_height};
    SDL_RenderCopyEx(
        renderer, m_texture, &m_rect, &renderQuad, angle, center, flip
    );
}

void Window::handle_events(SDL_Event event) {
    if (!m_is_active) {
        return;
    }
    if (!m_active_window.empty()) {
        m_windows[m_active_window]->handle_events(event);
    }
    switch (event.type) {
        case SDL_TEXTINPUT:
            if (!m_active_text_field.empty() &&
                (((SDL_GetModState() & KMOD_CTRL) == 0) ||
                 (event.text.text[0] != 'c' && event.text.text[0] != 'C' &&
                  event.text.text[0] != 'v' && event.text.text[0] != 'V'))) {
                m_text_fields[m_active_text_field].push(event.text.text);
            }
            break;
        case SDL_KEYDOWN:
            if (m_active_text_field.empty()) {
                break;
            }
            if (event.key.keysym.sym == SDLK_BACKSPACE) {
                m_text_fields[m_active_text_field].pop();
            } else if (event.key.keysym.sym == SDLK_c && ((SDL_GetModState() & KMOD_CTRL) != 0)) {
                SDL_SetClipboardText(
                    m_text_fields[m_active_text_field].get().c_str()
                );
            } else if (event.key.keysym.sym == SDLK_v && ((SDL_GetModState() & KMOD_CTRL) != 0)) {
                m_text_fields[m_active_text_field].clear();
                m_text_fields[m_active_text_field].push(SDL_GetClipboardText());
            }
            break;
    }
}

void Window::update(Point mouse_pos, bool mouse_pressed) {
    if (!m_is_active) {
        return;
    }
    for (const auto &[name, window] : m_windows) {
        if (m_window_updatable[name] &&
            window->in_bounds(
                mouse_pos -
                Point(m_window_pos[name].x(), m_window_pos[name].y())
            ) &&
            mouse_pressed) {
            set_active_window(name);
            window->update(
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
            }
        }
    }
    if (mouse_pressed) {
        reset_active_window();
        reset_active_text_field();
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
            }
        }
    }
}

void Window::render_texture(
    SDL_Renderer *renderer,
    int x_offset,
    int y_offset,
    int width,
    int height,
    SDL_Texture *texture,
    SDL_Texture *main_texture,
    SDL_Rect *clip,
    double angle,
    SDL_Point *center,
    SDL_RendererFlip flip
) const {
    SDL_SetRenderTarget(renderer, m_texture);
    const SDL_Rect renderQuad = {x_offset, y_offset, width, height};
    SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip);
    SDL_SetRenderTarget(renderer, main_texture);
}

}  // namespace runebound::graphics