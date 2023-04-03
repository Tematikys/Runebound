#ifndef RUNEBOUND_GRAPHICS_BOARD_HPP_
#define RUNEBOUND_GRAPHICS_BOARD_HPP_

#include <SDL2/SDL.h>
#include <graphics_point.hpp>
#include <graphics_segment.hpp>
#include <graphics_shapes.hpp>
#include <map_client.hpp>
#include <optional>
#include <vector>

namespace runebound::graphics {
// board class, contains hexagons, has connected methods
class Board {
private:
    // tokens
    ::std::vector<CircleShape> m_circles;
    ::std::vector<SDL_Color> m_circle_fill_color;
    ::std::vector<SDL_Color> m_circle_border_color;
    ::std::size_t m_circle_amount{0};
    ::std::size_t m_selected_circle{0xFFFF};

    // rivers
    ::std::vector<Segment> m_segments;
    ::std::vector<SDL_Color> m_segment_color;
    ::std::size_t m_segment_amount{0};

    // cells
    ::std::vector<HexagonShape> m_hexagons;
    ::std::vector<SDL_Color> m_hexagon_fill_colors;
    ::std::vector<SDL_Color> m_hexagon_border_color;
    ::std::size_t m_hexagon_amount{0};
    ::std::size_t m_selected_hexagon{0xFFFF};

public:
    Board() = default;

    explicit Board(const ::runebound::map::MapClient &map);

    void add_hexagon(
        HexagonShape &hex,
        SDL_Color fill_color,
        SDL_Color border_color
    );

    void add_segment(Segment &seg, SDL_Color col);

    void
    add_circle(CircleShape &cir, SDL_Color fill_color, SDL_Color border_color);

    void render(SDL_Renderer *renderer) const;

    void update_selection(const Point &dot);
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_BOARD_HPP_
