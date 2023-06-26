#include <cmath>
#include <graphics_board.hpp>

namespace {
int sign(int x) {
    return static_cast<int>(x > 0) - static_cast<int>(x < 0);
}
}  // namespace

namespace runebound::graphics {
Board::Board(const ::runebound::map::MapClient &map) {
    for (int row = 0; row < ::runebound::map::STANDARD_SIZE; ++row) {
        for (int col = 0; col < ::runebound::map::STANDARD_SIZE; ++col) {
            add_cell(map, row, col);
            add_special(map, row, col);
            add_token(map, row, col);
            add_road(map, row, col);
        }
    }
    add_rivers(map);
}

void Board::add_rivers(const map::MapClient &map) {
    for (const auto &pair : map.m_rivers) {
        const SDL_Color river_color = {55, 26, 250, 255};
        const auto [x1, y1] = pair.first;
        const auto [x2, y2] = pair.second;
        const auto [i, v] = *RIVER_DIRECTIONS.find(
            {sign(x1 - x2) + (1 - std::abs(sign(x1 - x2))) * (2 * (y1 % 2) - 1),
             sign(y1 - y2)}
        );
        const HexagonShape hex =
            m_cells[x1 * ::runebound::map::STANDARD_SIZE + y1];
        const Segment seg = {hex.get_vertex(v.first), hex.get_vertex(v.second)};
        add_river(seg, river_color);
    }
}

void Board::add_river(const Segment &segment, SDL_Color color) {
    m_rivers.push_back(segment);
    m_river_color.push_back(color);
    ++m_river_amount;
}

void Board::add_cell(const ::runebound::map::MapClient &map, int row, int col) {
    const auto center = get_center_of_hexagon(row, col);
    const auto [type_cell_key, cell_fill_color] =
        *CELL_FILL_COLOR.find(map.m_map[row][col].get_type_cell());
    m_cells.emplace_back(center, HEXAGON_RADIUS);
    m_cell_fill_color.push_back(cell_fill_color);
    m_cell_border_color.push_back({0, 0, 0, 255});
    ++m_cell_amount;
    m_width = std::max(m_width, m_cells.back().get_vertex(2).x());
    m_height = std::max(m_height, m_cells.back().get_vertex(3).y());
}

void Board::add_token(
    const ::runebound::map::MapClient &map,
    int row,
    int col
) {
    const auto center = get_center_of_hexagon(row, col);
    const auto cell = map.m_map[row][col];
    if (cell.get_token() != ::runebound::AdventureType::NOTHING) {
        SDL_Color color;
        if (cell.get_side_token() == ::runebound::Side::FRONT) {
            color = (*ADVENTURE_COLOR.find(cell.get_token())).second;
        } else {
            color = (*USED_ADVENTURE_COLOR.find(cell.get_token())).second;
        }
        m_tokens.emplace_back(center, HEXAGON_RADIUS / 2);
        m_token_fill_color.push_back(color);
        m_token_border_color.push_back({0, 0, 0, 255});
        ++m_token_amount;
    }
}

void Board::add_special(
    const ::runebound::map::MapClient &map,
    int row,
    int col
) {
    const auto special = map.m_map[row][col].get_special_type_cell();
    if (special != ::runebound::map::SpecialTypeCell::NOTHING) {
        const auto center = get_center_of_hexagon(row, col);
        const auto [special_type_cell_key, img] = *SPECIAL_TO_STR.find(special);
        m_specials.push_back(img);
        m_specials_pos.push_back(center - Point(13, 13));
        ++m_special_amount;
    }
}

void Board::add_road(const ::runebound::map::MapClient &map, int row, int col) {
    if (map.m_map[row][col].check_road()) {
        const auto center = get_center_of_hexagon(row, col);
        for (auto [i, j] :
             map.get_all_neighbours(::runebound::Point(row, col))) {
            if (map.m_map[i][j].check_road() ||
                map.m_map[i][j].get_type_cell() ==
                    ::runebound::map::TypeCell::TOWN) {
                const Segment seg(center, get_center_of_hexagon(i, j));
                m_roads.push_back(seg);
                m_road_color.push_back({0x80, 0x80, 0x80, 0xFF});
                ++m_road_amount;
                m_is_connected_to_town.push_back(
                    map.m_map[i][j].get_type_cell() ==
                    ::runebound::map::TypeCell::TOWN
                );
            }
        }
    }
}

void Board::render(
    SDL_Renderer *renderer,
    int x_offset,
    int y_offset,
    std::map<std::string, Texture> &images
) const {
    for (std::size_t i = 0; i < m_cell_amount; ++i) {
        m_cells[i].render(renderer, x_offset, y_offset, m_cell_fill_color[i]);
        m_cells[i].render_border(
            renderer, x_offset, y_offset, m_cell_border_color[i], 1
        );
    }
    if (m_selected_cell != 0xFFFF && m_selected_token == 0xFFFF) {
        m_cells[m_selected_cell].render(
            renderer, x_offset, y_offset, SELECTED_COLOR
        );
        m_cells[m_selected_cell].render_border(
            renderer, x_offset, y_offset, m_cell_border_color[m_selected_cell],
            1
        );
    }
    for (const auto &e : m_available_hexagons) {
        auto center = get_center_of_hexagon(e.x(), e.y());
        const auto hex = HexagonShape(center, HEXAGON_RADIUS - 2);
        hex.render_border(
            renderer, x_offset, y_offset, {0xFF, 0x00, 0x00, 0xFF}, 3
        );
    }
    for (std::size_t i = 0; i < m_river_amount; ++i) {
        m_rivers[i].render(renderer, m_river_color[i], 5, x_offset, y_offset);
    }
    for (std::size_t i = 0; i < m_road_amount; ++i) {
        if (m_is_connected_to_town[i]) {
            m_roads[i].half_render(
                renderer, m_road_color[i], 7, x_offset, y_offset
            );
        } else {
            m_roads[i].render(renderer, m_road_color[i], 7, x_offset, y_offset);
        }
    }
    for (std::size_t i = 0; i < m_special_amount; ++i) {
        images[m_specials[i]].render(
            renderer, x_offset + m_specials_pos[i].x(),
            y_offset + m_specials_pos[i].y()
        );
    }
    for (std::size_t i = 0; i < m_token_amount; ++i) {
        m_tokens[i].render(renderer, m_token_fill_color[i], x_offset, y_offset);
        m_tokens[i].render_border(
            renderer, m_token_border_color[i], x_offset, y_offset
        );
    }
    if (m_selected_token != 0xFFFF) {
        m_tokens[m_selected_token].render(
            renderer, SELECTED_COLOR, x_offset, y_offset
        );
        m_tokens[m_selected_token].render_border(
            renderer, m_token_border_color[m_selected_token], x_offset, y_offset
        );
    }
}

void Board::render_to_texture(
    SDL_Renderer *renderer,
    SDL_Texture *&texture,
    std::map<std::string, Texture> &images
) const {
    SDL_DestroyTexture(texture);
    texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        m_width + 1, m_height + 1
    );
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(renderer);
    render(renderer, 0, 0, images);
    SDL_SetRenderTarget(renderer, nullptr);
}

void Board::update_selection(const Point &dot) {
    m_selected_cell = 0xFFFF;
    m_selected_token = 0xFFFF;
    for (std::size_t i = 0; i < m_cell_amount; ++i) {
        if (m_cells[i].in_bounds(dot)) {
            m_selected_cell = i;
            break;
        }
    }
    for (std::size_t i = 0; i < m_token_amount; ++i) {
        if (m_tokens[i].in_bounds(dot)) {
            m_selected_token = i;
            break;
        }
    }
}
}  // namespace runebound::graphics