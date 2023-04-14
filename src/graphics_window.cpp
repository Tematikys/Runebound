#include <graphics_window.hpp>

namespace runebound::graphics {
void Window::render(SDL_Renderer *renderer) const {
    for(::std::size_t i = 0; i < m_texture_amount; ++i) {
        m_textures[i].render(renderer, m_x,m_y);
    }
    for(const auto & button : m_buttons) {
        button.render(renderer, m_x, m_y);
    }
}
}