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
    ::std::vector<CircleShape> m_tokens;
    ::std::vector<SDL_Color> m_token_fill_color;
    ::std::vector<SDL_Color> m_token_border_color;
    ::std::size_t m_token_amount{0};
    ::std::size_t m_selected_token{0xFFFF};

    // rivers
    ::std::vector<Segment> m_rivers;
    ::std::vector<SDL_Color> m_river_color;
    ::std::size_t m_river_amount{0};
    // TODO
    ::std::vector<bool> is_connected_to_city;

    // specials
    ::std::vector<PolygonShape> m_specials;
    ::std::vector<SDL_Color> m_special_fill_color;
    ::std::vector<SDL_Color> m_special_border_color;
    ::std::size_t m_special_amount{0};

    // roads
    ::std::vector<Segment> m_roads;
    ::std::vector<SDL_Color> m_road_color;
    ::std::size_t m_road_amount{0};

    // cells
    ::std::vector<HexagonShape> m_cells;
    ::std::vector<SDL_Color> m_cell_fill_color;
    ::std::vector<SDL_Color> m_cell_border_color;
    ::std::size_t m_cell_amount{0};
    ::std::size_t m_selected_cell{0xFFFF};

public:
    Board() = default;

    explicit Board(const ::runebound::map::MapClient &map);

    void add_cell(HexagonShape &hex, SDL_Color fill_col, SDL_Color border_col);

    void add_river(Segment &seg, SDL_Color col);

    void add_road(Segment &seg, SDL_Color col);

    void
    add_token(CircleShape &cir, SDL_Color fill_col, SDL_Color border_col);

    void
    add_special(PolygonShape &poly, SDL_Color fill_col, SDL_Color border_col);

    void render(SDL_Renderer *renderer) const;

    void update_selection(const Point &dot);
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_BOARD_HPP_
