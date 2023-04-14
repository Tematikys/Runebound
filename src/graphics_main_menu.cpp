#include <graphics_main_menu.hpp>

namespace runebound::graphics {
void MainMenu::render(SDL_Renderer *renderer) {
    for (const auto &but : m_buttons) {
        but.render(renderer);
    }
    for (::std::size_t i = 0; i < m_texture_amount; ++i) {
        m_textures[i].render(
            renderer, m_texture_pos[i].x() + m_x, m_texture_pos[i].y() + m_y
        );
    }
}
}  // namespace runebound::graphics