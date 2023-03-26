#include <graphics_board.hpp>
#include <map>

namespace {
int sign(int x) {
    if (x < 0) {
        return -1;
    }
    if (x > 0) {
        return 1;
    }
    return 0;
}
}  // namespace

namespace runebound::graphics {
// constants for hexagons
const SDL_Color SELECTED_COLOR = {0xFF, 0xF7, 0x00, 0xFF};
const int HEXAGON_RADIUS = 40;

// constants for rivers
const ::std::map<::std::pair<int, int>, ::std::pair<int, int>>
    RIVER_DIRECTIONS = {{{1, -1}, {0, 1}},  {{0, -1}, {1, 2}},
                        {{-1, -1}, {2, 3}}, {{-1, 1}, {3, 4}},
                        {{0, 1}, {4, 5}},   {{1, 1}, {5, 0}}};

Board::Board(const ::runebound::map::Map &map) {
    for (int row = 0; row < ::runebound::map::STANDARD_SIZE; ++row) {
        for (int col = 0; col < ::runebound::map::STANDARD_SIZE; ++col) {
            // get necessary color
            SDL_Color fill_color;
            switch (map.get_cell_map(::Point(row, col)).get_type_cell()) {
                case ::runebound::map::TypeCell::WATER:
                    fill_color = {0x37, 0x1A, 0xCA, 0xFF};
                    break;
                case ::runebound::map::TypeCell::FOREST:
                    fill_color = {0x15, 0x87, 0x1D, 0xFF};
                    break;
                case ::runebound::map::TypeCell::MOUNTAINS:
                    fill_color = {0x68, 0x7C, 0x7C, 0xFF};
                    break;
                case ::runebound::map::TypeCell::HILLS:
                    fill_color = {0x72, 0xB0, 0x34, 0xFF};
                    break;
                case ::runebound::map::TypeCell::PLAIN:
                    fill_color = {0x11, 0xF0, 0x4D, 0xFF};
                    break;
                case ::runebound::map::TypeCell::TOWN:
                    fill_color = {0x03, 0x07, 0x06, 0xFF};
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

            add_hexagon(hex, fill_color, SDL_Color{0x00, 0x00, 0x00, 0xFF});
        }
    }
    for (const auto &pair : map.get_rivers()) {
        SDL_Color river_color = {0x37, 0x1A, 0xFA, 0xFF};
        auto [x1, y1] = pair.first;
        auto [x2, y2] = pair.second;
        auto [i, v] = *RIVER_DIRECTIONS.find(
            {sign(x1 - x2), sign((2 * y1 + 1 - x1 % 2) - (2 * y2 + 1 - x2 % 2))}
        );
        HexagonShape hex =
            m_hexagons[x1 * ::runebound::map::STANDARD_SIZE + y1];
        Segment seg = {hex.get_vertex(v.first), hex.get_vertex(v.second)};
        add_segment(seg, river_color);
    }
}

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

void Board::add_segment(Segment &seg, SDL_Color col) {
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
    for (::std::size_t i = 0; i < m_segment_amount; ++i) {
        m_segments[i].render(renderer, m_segment_color[i]);
    }
}

::std::optional<::std::size_t> Board::in_bounds(const Point &dot) const {
    for (::std::size_t i = 0; i < m_hexagon_amount; ++i) {
        if (m_hexagons[i].in_bounds(dot)) {
            return i;
        }
    }
    return {};
}
}  // namespace runebound::graphics