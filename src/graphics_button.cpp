#include <graphics_button.hpp>

namespace runebound::graphics {
RectButton::RectButton(
    int x,
    int y,
    int width,
    int height,
    int texture_x_offset,
    int texture_y_offset,
    Texture &texture,
    ::std::function<void()> on_click_function,
    ::std::function<void()> on_cover_function,
    SDL_Color fill_color,
    SDL_Color border_color
)
    : m_x(x),
      m_y(y),
      m_width(width),
      m_height(height),
      m_texture_x_offset(texture_x_offset),
      m_texture_y_offset(texture_y_offset),
      m_texture(::std::move(texture)),
      m_on_click_function(::std::move(on_click_function)),
      m_on_cover_function(::std::move(on_cover_function)),
      m_fill_color(fill_color),
      m_border_color(border_color),
      m_shape({x, y, m_width, m_height}) {
}

bool RectButton::in_bounds(const Point &p) const {
    if (m_x <= p.x() && p.x() < m_x + m_width && m_y <= p.y() &&
        p.y() < m_y + m_height) {
        return true;
    }
    return false;
}

void RectButton::render(SDL_Renderer *renderer) const {
    m_shape.render(renderer, m_fill_color);
    m_shape.render_border(renderer, m_border_color);
    m_texture.render(
        renderer, m_x + m_texture_x_offset, m_y + m_texture_y_offset
    );
}

CircButton::CircButton(
    int x,
    int y,
    int width,
    int height,
    int texture_x_offset,
    int texture_y_offset,
    Texture &texture,
    ::std::function<void()> on_click_function,
    ::std::function<void()> on_cover_function,
    SDL_Color fill_color,
    SDL_Color border_color
)
    : m_x(x),
      m_y(y),
      m_width(width),
      m_height(height),
      m_texture_x_offset(texture_x_offset),
      m_texture_y_offset(texture_y_offset),
      m_texture(::std::move(texture)),
      m_on_click_function(::std::move(on_click_function)),
      m_on_cover_function(::std::move(on_cover_function)),
      m_fill_color(fill_color),
      m_border_color(border_color) {
    m_center = Point(x + m_width / 2, y + m_height / 2);
    m_radius = (m_center.x() + m_center.y() - x - y) / 2;
    m_shape = CircleShape(m_center, m_radius);
}

bool CircButton::in_bounds(const Point &p) const {
    int dx = p.x() - m_center.x();
    int dy = p.y() - m_center.y();
    if (dx * dx + dy * dy <= m_radius * m_radius) {
        return true;
    }
    return false;
}

void CircButton::render(SDL_Renderer *renderer) const {
    m_shape.render(renderer, m_fill_color);
    m_shape.render_border(renderer, m_border_color);
    m_texture.render(
        renderer, m_x + m_texture_x_offset, m_y + m_texture_y_offset
    );
}
}  // namespace runebound::graphics