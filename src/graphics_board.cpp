#include <cmath>
#include <graphics_board.hpp>

namespace {
int sign(int x) {
    return (x > 0) - (x < 0);
}
}  // namespace

namespace runebound::graphics {
Board::Board(const ::runebound::map::MapClient &map) {
    int couter = 0;
    for (int row = 0; row < ::runebound::map::STANDARD_SIZE; ++row) {
        for (int col = 0; col < ::runebound::map::STANDARD_SIZE; ++col) {
            auto center = get_center_of_hexagon(row, col);

            // add cell
            auto [type_cell_key, cell_fill_color] =
                *CELL_FILL_COLOR.find(map.m_map[row][col].get_type_cell());
            add_cell(
                {center, HEXAGON_RADIUS}, cell_fill_color,
                SDL_Color{0x00, 0x00, 0x00, 0xFF}
            );

            // add special if it is
            ::runebound::map::SpecialTypeCell special;
            if ((special = map.m_map[row][col].get_special_type_cell()) !=
                ::runebound::map::SpecialTypeCell::NOTHING) {
                ++couter;
                auto [special_type_cell_key, special_fill_color] =
                    *SPECIAL_COLOR.find(special);
                add_special(
                    {center + Point(HEXAGON_RADIUS / 2, 0), HEXAGON_RADIUS / 5},
                    special_fill_color, {0x00, 0x00, 0x00, 0xFF}
                );
            }

            // add token if there is
            ::runebound::AdventureType token;
            if ((token = map.m_map[row][col].get_token()) !=
                ::runebound::AdventureType::NOTHING) {
                add_token(
                    {center, HEXAGON_RADIUS / 2},
                    (*ADVENTURE_COLOR.find(token)).second,
                    {0x00, 0x00, 0x00, 0xFF}
                );
            }

            // adding roads if is it TODO
            if (map.m_map[row][col].check_road()) {
                for (auto [i, j] :
                     map.get_all_neighbours(::runebound::Point(row, col))) {
                    if (map.m_map[i][j].check_road()) {
                        Segment seg(center, get_center_of_hexagon(i, j));
                        add_road(seg, {0x80, 0x80, 0x80, 0xFF});
                        m_is_connected_to_town.push_back(false);
                    }
                    if (map.m_map[i][j].get_type_cell() ==
                        ::runebound::map::TypeCell::TOWN) {
                        Segment seg(center, get_center_of_hexagon(i, j));
                        add_road(seg, {0x80, 0x80, 0x80, 0xFF});
                        m_is_connected_to_town.push_back(true);
                    }
                }
            }
        }
    }

    // adding rivers
    for (const auto &pair : map.m_rivers) {
        SDL_Color river_color = {0x37, 0x1A, 0xFA, 0xFF};
        auto [x1, y1] = pair.first;
        auto [x2, y2] = pair.second;
        auto [i, v] = *RIVER_DIRECTIONS.find(
            {sign(x1 - x2) +
                 (1 - ::std::abs(sign(x1 - x2))) * (2 * (y1 % 2) - 1),
             sign(y1 - y2)}
        );

        HexagonShape hex = m_cells[x1 * ::runebound::map::STANDARD_SIZE + y1];
        Segment seg = {hex.get_vertex(v.first), hex.get_vertex(v.second)};
        add_river(seg, river_color);
    }
    ::std::cout << m_specials.size() << ' ' << couter << ::std::endl;
}

void Board::add_cell(
    const HexagonShape &hexagon,
    SDL_Color fill_color,
    SDL_Color border_color
) {
    m_cells.push_back(hexagon);
    m_cell_fill_color.push_back(fill_color);
    m_cell_border_color.push_back(border_color);
    ++m_cell_amount;
}

void Board::add_river(const Segment &segment, SDL_Color color) {
    m_rivers.push_back(segment);
    m_river_color.push_back(color);
    ++m_river_amount;
}

void Board::add_road(const Segment &segment, SDL_Color color) {
    m_roads.push_back(segment);
    m_road_color.push_back(color);
    ++m_road_amount;
}

void Board::add_token(
    const CircleShape &circle,
    SDL_Color fill_color,
    SDL_Color border_color
) {
    m_tokens.push_back(circle);
    m_token_fill_color.push_back(fill_color);
    m_token_border_color.push_back(border_color);
    ++m_token_amount;
}

void Board::add_special(
    const SquareShape &square,
    SDL_Color fill_color,
    SDL_Color border_color
) {
    m_specials.push_back(square);
    m_special_fill_color.push_back(fill_color);
    m_special_border_color.push_back(border_color);
    ++m_special_amount;
}

void Board::render(SDL_Renderer *renderer) const {
    // cells
    for (::std::size_t i = 0; i < m_cell_amount; ++i) {
        m_cells[i].render(renderer, m_cell_fill_color[i]);
        m_cells[i].render_border(renderer, m_cell_border_color[i]);
    }
    if (m_selected_cell != 0xFFFF && m_selected_token == 0xFFFF) {
        m_cells[m_selected_cell].render(renderer, SELECTED_COLOR);
        m_cells[m_selected_cell].render_border(
            renderer, m_cell_border_color[m_selected_cell]
        );
    }

    // rivers TODO
    for (::std::size_t i = 0; i < m_river_amount; ++i) {
        m_rivers[i].render(renderer, m_river_color[i], 5);
    }
    for (::std::size_t i = 0; i < m_road_amount; ++i) {
        if (m_is_connected_to_town[i]) {
            m_roads[i].half_render(renderer, m_road_color[i], 7);
        } else {
            m_roads[i].render(renderer, m_road_color[i], 7);
        }
    }

    // specials
    for (::std::size_t i = 0; i < m_special_amount; ++i) {
        m_specials[i].render(renderer, m_special_fill_color[i]);
        m_specials[i].render_border(renderer, m_special_border_color[i]);
    }

    // tokens
    for (::std::size_t i = 0; i < m_token_amount; ++i) {
        m_tokens[i].render(renderer, m_token_fill_color[i]);
        m_tokens[i].render_border(renderer, m_token_border_color[i]);
    }
    if (m_selected_token != 0xFFFF) {
        m_tokens[m_selected_token].render(renderer, SELECTED_COLOR);
        m_tokens[m_selected_token].render_border(
            renderer, m_token_border_color[m_selected_token]
        );
    }
}

void Board::update_selection(const Point &dot) {
    m_selected_cell = 0xFFFF;
    m_selected_token = 0xFFFF;
    for (::std::size_t i = 0; i < m_cell_amount; ++i) {
        if (m_cells[i].in_bounds(dot)) {
            m_selected_cell = i;
            break;
        }
    }
    for (::std::size_t i = 0; i < m_token_amount; ++i) {
        if (m_tokens[i].in_bounds(dot)) {
            m_selected_token = i;
            break;
        }
    }
}
}  // namespace runebound::graphics