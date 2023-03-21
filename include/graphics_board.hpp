#ifndef RUNEBOUND_GRAPHICS_BOARD_HPP_
#define RUNEBOUND_GRAPHICS_BOARD_HPP_

#include <SDL2/SDL.h>
#include <graphics_point.hpp>
#include <graphics_shapes.hpp>
#include <map.hpp>
#include <optional>
#include <vector>

namespace runebound::graphics {
// board class, contains hexagons, has connected methods
class Board {
private:
    ::std::vector<HexagonShape> m_hexagons;
    ::std::vector<SDL_Color> m_fill_colors;
    ::std::vector<SDL_Color> m_border_color;
    ::std::size_t m_hexagon_amount{0};
    ::std::size_t m_selected_hexagon{0xFFFF};

public:
    Board() = default;

    explicit Board(const ::runebound::map::Map &map);

    void add_hexagon(
        HexagonShape &hex,
        SDL_Color fill_color,
        SDL_Color border_color
    );

    void render(SDL_Renderer *renderer) const;

    void set_selected_hexagon(::std::size_t index) {
        m_selected_hexagon = index;
    };

    [[nodiscard]] ::std::optional<::std::size_t> in_bounds(Point dot) const;
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_BOARD_HPP_
