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

Button::Button(Button &&other) noexcept
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

Button &Button::operator=(Button &&other) noexcept {
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

void Button::render(SDL_Renderer *renderer) const {
    m_shape.render(renderer, m_fill_color);
    m_shape.render_border(renderer, m_border_color);
    m_texture.render(
        renderer, m_x + m_texture_x_offset, m_y + m_texture_y_offset
    );
}

bool Button::in_bounds(const Point &p) const {
    return m_x <= p.x() && p.x() < m_x + m_width && m_y <= p.y() &&
           p.y() < m_y + m_height;
}

TextField::TextField(::std::string text, Button &button, int max_len)
    : m_text(::std::move(text)),
      m_button(::std::move(button)),
      m_max_text_len(max_len) {
}

void TextField::push(const ::std::string &suffix) {
    if (m_max_text_len == 0) {
        m_text += suffix;
    } else {
        if (suffix.length() + m_text.length() <= m_max_text_len) {
            m_text += suffix;
        }
    }
}

void TextField::render(
    SDL_Renderer *renderer,
    TTF_Font *font,
    SDL_Color color,
    SDL_Rect *clip,
    double angle,
    SDL_Point *center,
    SDL_RendererFlip flip
) const {
    m_button.render(renderer);
    Texture texture;
    generate_text(renderer, texture, m_text, font, color);
    texture.render(
        renderer, m_button.get_coords().x(), m_button.get_coords().y(), clip,
        angle, center, flip
    );
    texture.free();
}
}  // namespace runebound::graphics