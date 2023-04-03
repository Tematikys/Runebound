#include <cmath>
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
const int HEXAGON_RADIUS = 30;

// constants for rivers
const ::std::map<::std::pair<int, int>, ::std::pair<int, int>>
    RIVER_DIRECTIONS = {{{-1, 0}, {3, 4}}, {{-1, -1}, {2, 3}},
                        {{-1, 1}, {4, 5}}, {{1, 0}, {0, 1}},
                        {{1, -1}, {1, 2}}, {{1, 1}, {5, 0}}};

Board::Board(const ::runebound::map::MapClient &map) {
    for (int row = 0; row < ::runebound::map::STANDARD_SIZE; ++row) {
        for (int col = 0; col < ::runebound::map::STANDARD_SIZE; ++col) {
            // get necessary color
            SDL_Color fill_color;
            switch (map.m_map[row][col].get_type_cell()) {
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
            const int dy = (HEXAGON_RADIUS * 56756) >> 16;

            Point center;
            if (col % 2 == 0) {
                center = Point(
                    HEXAGON_RADIUS * (2 + col * 3) / 2, dy * (1 + 2 * row)
                );
            } else {
                center = Point(
                    HEXAGON_RADIUS * (2 + col * 3) / 2, dy * 2 * (1 + row)
                );
            }
            HexagonShape hex = {center, HEXAGON_RADIUS};

            add_hexagon(hex, fill_color, SDL_Color{0x00, 0x00, 0x00, 0xFF});
            bool tr = true;
            switch (map.m_map[row][col].get_token()) {
                case ::runebound::AdventureType::MEETING:
                    fill_color = {0x9D, 0x00, 0xC4, 0xFF};
                    break;
                case AdventureType::RESEARCH:
                    fill_color = {0x00, 0x7F, 0x00, 0xFF};
                    break;
                case AdventureType::FIGHT:
                    fill_color = {0xC4, 0x90, 0x00, 0xFF};
                    break;
                case AdventureType::NOTHING:
                    tr = false;
                    break;
            }
            if (tr) {
                CircleShape cir = {center, HEXAGON_RADIUS / 2};
                add_circle(cir, fill_color, {0x00, 0x00, 0x00, 0xFF});
            }
        }
    }
    for (const auto &pair : map.m_rivers) {
        SDL_Color river_color = {0x37, 0x1A, 0xFA, 0xFF};
        auto [x1, y1] = pair.first;
        auto [x2, y2] = pair.second;
        auto [i, v] = *RIVER_DIRECTIONS.find(
            {sign(x1 - x2) +
                 (1 - ::std::abs(sign(x1 - x2))) * (2 * (y1 % 2) - 1),
             sign(y1 - y2)}
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

void Board::add_circle(
    CircleShape &cir,
    SDL_Color fill_color,
    SDL_Color border_color
) {
    m_circles.push_back(::std::move(cir));
    m_circle_fill_color.push_back(fill_color);
    m_circle_border_color.push_back(border_color);
    ++m_circle_amount;
}

void Board::render(SDL_Renderer *renderer) const {
    for (::std::size_t i = 0; i < m_hexagon_amount; ++i) {
        m_hexagons[i].render(
            renderer, m_hexagon_fill_colors[i], m_hexagon_border_color[i]
        );
    }
    if (m_selected_hexagon != 0xFFFF && m_selected_circle == 0xFFFF) {
        m_hexagons[m_selected_hexagon].render(
            renderer, SELECTED_COLOR, m_hexagon_border_color[m_selected_hexagon]
        );
    }
    for (::std::size_t i = 0; i < m_segment_amount; ++i) {
        m_segments[i].render(renderer, m_segment_color[i]);
    }

    for (::std::size_t i = 0; i < m_circle_amount; ++i) {
        m_circles[i].render(
            renderer, m_circle_fill_color[i], m_circle_border_color[i]
        );
    }
    if (m_selected_circle != 0xFFFF) {
        m_circles[m_selected_circle].render(
            renderer, SELECTED_COLOR, m_circle_border_color[m_selected_circle]
        );
    }
}

void Board::update_selection(const Point &dot) {
    m_selected_hexagon = 0xFFFF;
    m_selected_circle = 0xFFFF;
    for (::std::size_t i = 0; i < m_hexagon_amount; ++i) {
        if (m_hexagons[i].in_bounds(dot)) {
            m_selected_hexagon = i;
            break;
        }
    }
    for (::std::size_t i = 0; i < m_circle_amount; ++i) {
        if (m_circles[i].in_bounds(dot)) {
            m_selected_circle = i;
            break;
        }
    }
}
}  // namespace runebound::graphics