#include <graphics_board.hpp>
#include <map.hpp>
#include <map_cell.hpp>

namespace runebound::graphics {
const SDL_Color SELECTED_COLOR = {0xFF, 0xF7, 0x00, 0xFF};
const int HEXAGON_RADIUS = 50;

void Board::add_hexagon(
    HexagonShape &hex,
    SDL_Color fill_color,
    SDL_Color border_color
) {
    m_hexagons.push_back(::std::move(hex));
    m_fill_colors.push_back(fill_color);
    m_border_color.push_back(border_color);
    ++m_hexagon_amount;
}

void Board::render(SDL_Renderer *renderer) const {
    for (::std::size_t i = 0; i < m_hexagon_amount; ++i) {
        if (i == m_selected_hexagon) {
            m_hexagons[i].render(renderer, SELECTED_COLOR, m_border_color[i]);
        } else {
            m_hexagons[i].render(renderer, m_fill_colors[i], m_border_color[i]);
        }
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
    for (int row = 0; row < ::runebound::map::StandartHeight; ++row) {
        for (int col = 0; col < ::runebound::map::StandartWidth; ++col) {
            // get necessary color
            SDL_Color color;
            switch (map.get_cell_map(row, col).get_type_cell()) {
                case ::runebound::map::TypeCell::WATER:
                    color = {0x00, 0x00, 0xFF, 0xFF};
                    break;
                case ::runebound::map::TypeCell::FOREST:
                    color = {0x00, 0xFF, 0x00, 0xFF};
                    break;
                case ::runebound::map::TypeCell::MOUNTAINS:
                    color = {0x77, 0x77, 0x77, 0xFF};
                    break;
                case ::runebound::map::TypeCell::HILLS:
                    color = {0x00, 0x77, 0x00, 0xFF};
                    break;
                case ::runebound::map::TypeCell::PLAIN:
                    color = {0x77, 0xFF, 0x77, 0xFF};
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
}
}  // namespace runebound::graphics