#include <graphics_board.hpp>

namespace runebound::graphics {
// constants for hexagons
const SDL_Color SELECTED_COLOR = {0xFF, 0xF7, 0x00, 0xFF};
const int HEXAGON_RADIUS = 40;

void Board::add_hexagon(
    HexagonShape &hex,
    SDL_Color fill_color,
    SDL_Color border_color
) {
    m_hexagons.push_back(::std::move(hex));
    m_hexagon_fill_colors.push_back(fill_color);
    m_hexagon_border_color.push_back(border_color);
    ++m_hexagon_amount;
}

void Board::add_segment(Segment seg, SDL_Color col) {
    m_segments.push_back(::std::move(seg));
    m_segment_color.push_back(col);
    ++m_segment_amount;
}

void Board::render(SDL_Renderer *renderer) const {
    for (::std::size_t i = 0; i < m_hexagon_amount; ++i) {
        if (i == m_selected_hexagon) {
            m_hexagons[i].render(
                renderer, SELECTED_COLOR, m_hexagon_border_color[i]
            );
        } else {
            m_hexagons[i].render(
                renderer, m_hexagon_fill_colors[i], m_hexagon_border_color[i]
            );
        }
    }
    for(::std::size_t i = 0; i < m_segment_amount; ++i) {
        m_segments[i].render(renderer, m_segment_color[i]);
    }
}

::std::optional<::std::size_t> Board::in_bounds(Point dot) const {
    for (::std::size_t i = 0; i < m_hexagon_amount; ++i) {
        if (m_hexagons[i].in_bounds(dot)) {
            return i;
        }
    }
    return {};
}

Board::Board(const ::runebound::map::Map &map) {
    for (int row = 0; row < ::runebound::map::STANDARD_SIZE; ++row) {
        for (int col = 0; col < ::runebound::map::STANDARD_SIZE; ++col) {
            // get necessary color
            SDL_Color color;
            switch (map.get_cell_map(::Point(row, col)).get_type_cell()) {
                case ::runebound::map::TypeCell::WATER:
                    color = {0x7F, 0x7F, 0xFF, 0xFF};
                    break;
                case ::runebound::map::TypeCell::FOREST:
                    color = {0x00, 0x77, 0x00, 0xFF};
                    break;
                case ::runebound::map::TypeCell::MOUNTAINS:
                    color = {0x77, 0x77, 0x77, 0xFF};
                    break;
                case ::runebound::map::TypeCell::HILLS:
                    color = {0x7F, 0x77, 0x00, 0xFF};
                    break;
                case ::runebound::map::TypeCell::PLAIN:
                    color = {0x77, 0xFF, 0x77, 0xFF};
                    break;
                case ::runebound::map::TypeCell::TOWN:
                    color = {0x00, 0x00, 0x00, 0xFF};
                    break;
            }

            // supportive variable
            const int dx =
                (::runebound::graphics::HEXAGON_RADIUS * 56756) >> 16;

            ::runebound::graphics::HexagonShape hex;
            if (row % 2 == 0) {
                hex = ::runebound::graphics::HexagonShape(
                    ::runebound::graphics::Point(
                        dx * 2 * (1 + col),
                        ::runebound::graphics::HEXAGON_RADIUS * (2 + row * 3) /
                            2
                    ),
                    ::runebound::graphics::HEXAGON_RADIUS
                );
            } else {
                hex = ::runebound::graphics::HexagonShape(
                    ::runebound::graphics::Point(
                        dx * (1 + 2 * col),
                        ::runebound::graphics::HEXAGON_RADIUS * (2 + row * 3) /
                            2
                    ),
                    ::runebound::graphics::HEXAGON_RADIUS
                );
            }

            add_hexagon(hex, color, SDL_Color{0x00, 0x00, 0x00, 0xFF});
        }
    }
    for (const auto &pair : map.get_rivers()) {
        int x1 = pair.first.x;
        int y1 = pair.first.y;
        int x2 = pair.second.x;
        int y2 = pair.second.y;
        if (x1 == x2) {
            HexagonShape hex =
                m_hexagons[x1 * ::runebound::map::STANDARD_SIZE + y1];
            if (y1 < y2) {
                add_segment(
                    {hex.get_vertex(1), hex.get_vertex(2)},
                    {0x00, 0x0F, 0xFF, 0xFF}
                );
            } else {
                add_segment(
                    {hex.get_vertex(5), hex.get_vertex(4)},
                    {0x00, 0x0F, 0xFF, 0xFF}
                );
            }
        }
        else if(x1 < x2) {
            HexagonShape hex =
                m_hexagons[x1 * ::runebound::map::STANDARD_SIZE + y1];
            if (y1 < y2) {
                add_segment(
                    {hex.get_vertex(0), hex.get_vertex(5)},
                    {0x00, 0x0F, 0xFF, 0xFF}
                );
            } else {
                add_segment(
                    {hex.get_vertex(0), hex.get_vertex(1)},
                    {0x00, 0x0F, 0xFF, 0xFF}
                );
            }
        }
        else {
            HexagonShape hex =
                m_hexagons[x1 * ::runebound::map::STANDARD_SIZE + y1];
            if (y1 < y2) {
                add_segment(
                    {hex.get_vertex(2), hex.get_vertex(3)},
                    {0x00, 0x0F, 0xFF, 0xFF}
                );
            } else {
                add_segment(
                    {hex.get_vertex(3), hex.get_vertex(4)},
                    {0x00, 0x0F, 0xFF, 0xFF}
                );
            }
        }
    }
}
}  // namespace runebound::graphics