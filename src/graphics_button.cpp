#include <graphics_button.hpp>

namespace runebound::graphics {
Button::Button(
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

bool Button::in_bounds(const Point &p) const {
    if (m_x <= p.x() && p.x() < m_x + m_width && m_y <= p.y() &&
        p.y() < m_y + m_height) {
        return true;
    }
    return false;
}

void Button::render(SDL_Renderer *renderer) const {
    m_shape.render(renderer, m_fill_color);
    m_shape.render_border(renderer, m_border_color);
    m_texture.render(
        renderer, m_x + m_texture_x_offset, m_y + m_texture_y_offset
    );
}

void TextField::render(
    SDL_Renderer *renderer,
    TTF_Font *font,
    SDL_Color color,
    int x,
    int y,
    SDL_Rect *clip,
    double angle,
    SDL_Point *center,
    SDL_RendererFlip flip
) const {
    m_button.render(renderer);
    Texture texture;
    generate_text(renderer, texture, m_text, font, color);
    texture.render(renderer, x, y, clip, angle, center, flip);
    texture.free();
}
}  // namespace runebound::graphics