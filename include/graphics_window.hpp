#ifndef RUNEBOUND_GRAPHICS_WINDOW_HPP_
#define RUNEBOUND_GRAPHICS_WINDOW_HPP_

#include <graphics.hpp>
#include <graphics_button.hpp>
#include <vector>

namespace runebound::graphics {
class Window {
private:
    int m_width, m_height, m_x, m_y;
    ::std::vector<Texture> m_textures;
    ::std::vector<Point> m_texture_coords;
    ::std::size_t m_texture_amount;
    ::std::vector<RectButton> m_buttons;

public:
    void render(SDL_Renderer *renderer) const;
};
}  // namespace runebound::graphics

#endif  // RUNEBOUND_GRAPHICS_WINDOW_HPP_