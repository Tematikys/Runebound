#include <graphics_button.hpp>

namespace runebound::graphics {
Button::Button(
    int x,
    int y,
    int width,
    int height,
    HorizontalButtonTextAlign hor_text_align,
    VerticalButtonTextAlign ver_text_align,
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
      m_hor_text_align(hor_text_align),
      m_ver_text_align(ver_text_align),
      m_texture_x_offset(texture_x_offset),
      m_texture_y_offset(texture_y_offset),
      m_texture(::std::move(texture)),
      m_on_click_function(::std::move(on_click_function)),
      m_on_cover_function(::std::move(on_cover_function)),
      m_fill_color(fill_color),
      m_border_color(border_color),
      m_shape({x, y, m_width, m_height}) {
    switch (m_hor_text_align) {
        case HorizontalButtonTextAlign::LEFT:
            m_texture_x_offset = 0;
            break;
        case HorizontalButtonTextAlign::CENTER:
            m_texture_x_offset = (m_width - m_texture.width()) / 2;
            break;
        case HorizontalButtonTextAlign::RIGHT:
            m_texture_x_offset = m_width - m_texture.width();
            break;
        case HorizontalButtonTextAlign::NONE:
            break;
    }
    switch (m_ver_text_align) {
        case VerticalButtonTextAlign::TOP:
            m_texture_y_offset = 0;
            break;
        case VerticalButtonTextAlign::CENTER:
            m_texture_y_offset = (m_height - m_texture.height()) / 2;
            break;
        case VerticalButtonTextAlign::BOTTOM:
            m_texture_x_offset = m_height - m_texture.height();
            break;
        case VerticalButtonTextAlign::NONE:
            break;
    }
}

Button::Button(Button &&other) noexcept
    : m_x(other.m_x),
      m_y(other.m_y),
      m_width(other.m_width),
      m_height(other.m_height),
      m_hor_text_align(other.m_hor_text_align),
      m_ver_text_align(other.m_ver_text_align),
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
    m_hor_text_align = other.m_hor_text_align;
    m_ver_text_align = other.m_ver_text_align;
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
    texture.load_text_from_string(renderer, font, m_text, color);
    texture.render(
        renderer, m_button.get_coords().x(), m_button.get_coords().y(), clip,
        angle, center, flip
    );
    texture.free();
}
}  // namespace runebound::graphics