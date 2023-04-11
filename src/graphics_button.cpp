#include <graphics_button.hpp>

namespace runebound::graphics {
RectButton::RectButton(
    int x,
    int y,
    Texture &tex,
    ::std::function<void()> func,
    SDL_Color fill_color,
    SDL_Color border_color
)
    : m_x(x),
      m_y(y),
      m_texture(::std::move(tex)),
      m_function(::std::move(func)),
      m_fill_color(fill_color),
      m_border_color(border_color) {
    m_width = m_texture.get_width();
    m_height = m_texture.get_height();
    m_shape = RectangleShape(x, y, m_width, m_height);
}

bool RectButton::in_bounds(const Point &p) const {
    if (m_x <= p.x() && p.x() < m_x + m_width && m_y <= p.y() &&
        p.y() < m_y + m_height) {
        return true;
    }
    return false;
}

CircButton::CircButton(
    int x,
    int y,
    Texture &tex,
    ::std::function<void()> func,
    SDL_Color fill_color,
    SDL_Color border_color
)
    : m_x(x),
      m_y(y),
      m_texture(::std::move(tex)),
      m_function(::std::move(func)),
      m_fill_color(fill_color),
      m_border_color(border_color) {
    m_width = m_texture.get_width();
    m_height = m_texture.get_height();
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
}  // namespace runebound::graphics