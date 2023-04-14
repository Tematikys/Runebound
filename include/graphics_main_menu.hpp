#ifndef RUNEBOUND_GRAPHICS_MAIN_MENU_HPP_
#define RUNEBOUND_GRAPHICS_MAIN_MENU_HPP_

#include <SDL2/SDL.h>
#include <graphics_button.hpp>
#include <graphics_config.hpp>
#include <vector>

namespace runebound::graphics {
class MainMenu {
private:
    int m_width, m_height, m_x, m_y;
    ::std::vector<Button> m_buttons;
    ::std::vector<Texture> m_textures;
    ::std::vector<Point> m_texture_pos;
    ::std::size_t m_texture_amount{0};

public:
    void render(SDL_Renderer *renderer);

    void add_button(Button &but) {
        m_buttons.emplace_back(::std::move(but));
    }

    void add_texture(Texture tex, int x, int y) {
        m_textures.push_back(::std::move(tex));
        m_texture_pos.emplace_back(x, y);
        ++m_texture_amount;
    };
};
}  // namespace runebound::graphics

#endif  // RUNEBOUND_GRAPHICS_MAIN_MENU_HPP_