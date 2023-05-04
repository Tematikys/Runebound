#include <graphics_window.hpp>

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

    for (::std::size_t i = 0; i < m_buttons.size(); ++i) {
        m_buttons[i].render(renderer, m_button_pos[i].x(), m_button_pos[i].y());
    }

    for (::std::size_t i = 0; i < m_text_fields.size(); ++i) {
        m_text_fields[i].render(
            renderer, m_text_field_fonts[i], m_text_field_colors[i],
            m_texture_pos[i].x(), m_text_field_pos[i].y()
        );
    }

    for (::std::size_t i = 0; i < m_textures.size(); ++i) {
        m_textures[i].render(
            renderer, m_texture_pos[i].x(), m_texture_pos[i].y()
        );
    }

    for (::std::size_t i = 0; i < m_textures.size(); ++i) {
        m_textures[i].render(
            renderer, m_texture_pos[i].x(), m_texture_pos[i].y()
        );
    }

    for (::std::size_t i = 0; i < m_windows.size(); ++i) {
        m_windows[i]->render(
            renderer, m_window_pos[i].x(), m_window_pos[i].y(), m_texture
        );
    }
    SDL_SetRenderTarget(renderer, main_texture);
    const SDL_Rect renderQuad = {x_offset, y_offset, m_width, m_height};
    SDL_RenderCopyEx(
        renderer, m_texture, &m_rect, &renderQuad, angle, center, flip
    );
}

void Window::click(Point mouse_pos) {
    for (::std::size_t i = 0; i < m_buttons.size(); ++i) {
        if (m_buttons[i].in_bounds(
                mouse_pos - Point(m_button_pos[i].x(), m_button_pos[i].x())
            )) {
            m_buttons[i].on_click();
            return;
        }
    }
    for (::std::size_t i = 0; i < m_text_fields.size(); ++i) {
        if (m_text_fields[i].in_bounds(
                mouse_pos -
                Point(m_text_field_pos[i].x(), m_text_field_pos[i].x())
            )) {
            m_text_fields[i].on_click();
            return;
        }
    }
    for (::std::size_t i = 0; i < m_windows.size(); ++i) {
        Point p = mouse_pos - Point(m_window_pos[i].x(), m_window_pos[i].x());
        if (m_windows[i]->in_bounds(p)) {
            m_windows[i]->click(p);
            return;
        }
    }
}
}  // namespace runebound::graphics