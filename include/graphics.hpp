#ifndef RUNEBOUND_GRAPHICS_HPP_
#define RUNEBOUND_GRAPHICS_HPP_

#include <graphics_config.hpp>
#include <graphics_point.hpp>
#include <string>

namespace runebound::graphics {
Point get_center_of_hexagon(int i, int j);

bool SDL_init(SDL_Window *&window, SDL_Renderer *&renderer);

void update_mouse_pos(Point &pos);

bool load_font(TTF_Font *&font, const ::std::string &path, int font_size);
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_HPP_