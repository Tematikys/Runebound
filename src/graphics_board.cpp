#include <graphics_board.hpp>

namespace runebound::graphics {
const SDL_Color SELECTED_COLOR = {0xFF, 0xF7, 0x00, 0xFF};

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
}  // namespace runebound::graphics