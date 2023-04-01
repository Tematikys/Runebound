#include <cmath>
#include <graphics_board.hpp>
#include <map>

namespace {
int sign(int x) {
    return (x > 0) - (x < 0);
}
}  // namespace

namespace runebound::graphics {
// constants for hexagons

const int HEXAGON_RADIUS = 36;

const SDL_Color SELECTED_COLOR = {0xFF, 0xF7, 0x00, 0xFF};

const ::std::map<::runebound::map::TypeCell, SDL_Color> CELL_FILL_COLOR = {
    {::runebound::map::TypeCell::WATER, {0x37, 0x1A, 0xCA, 0xFF}},
    {::runebound::map::TypeCell::FOREST, {0x15, 0x66, 0x1D, 0xFF}},
    {::runebound::map::TypeCell::MOUNTAINS, {0x68, 0x7C, 0x7C, 0xFF}},
    {::runebound::map::TypeCell::HILLS, {0x72, 0xB0, 0x34, 0xFF}},
    {::runebound::map::TypeCell::PLAIN, {0x11, 0xF0, 0x4D, 0xFF}},
    {::runebound::map::TypeCell::TOWN, {0x03, 0x07, 0x06, 0xFF}}};

const ::std::map<::runebound::map::SpecialTypeCell, SDL_Color> SPECIAL_COLOR = {
    {::runebound::map::SpecialTypeCell::SETTLEMENT, {0xE2, 0xE2, 0x18, 0xFF}},
    {::runebound::map::SpecialTypeCell::SANCTUARY, {0xFF, 0xFF, 0xFF, 0xFF}},
    {::runebound::map::SpecialTypeCell::FORTRESS, {0xA0, 0xA0, 0xA0, 0xFF}}};

const ::std::map<::runebound::AdventureType, SDL_Color> ADVENTURE_COLOR = {
    {::runebound::AdventureType::MEETING, {0x9D, 0x00, 0xC4, 0xFF}},
    {::runebound::AdventureType::RESEARCH, {0x00, 0x9F, 0x00, 0xFF}},
    {::runebound::AdventureType::FIGHT, {0xC4, 0x90, 0x00, 0xFF}}};

// constants for rivers

const ::std::map<::std::pair<int, int>, ::std::pair<int, int>>
    RIVER_DIRECTIONS = {{{-1, 0}, {3, 4}}, {{-1, -1}, {2, 3}},
                        {{-1, 1}, {4, 5}}, {{1, 0}, {0, 1}},
                        {{1, -1}, {1, 2}}, {{1, 1}, {5, 0}}};

// =============================================================================
// TODO
Point<int> centerize(int i, int j) {
    static const int dy = (HEXAGON_RADIUS * 56756) >> 16;
    if (j % 2 == 0) {
        return {HEXAGON_RADIUS * (2 + j * 3) / 2, dy * (1 + 2 * i)};
    }
    return {HEXAGON_RADIUS * (2 + j * 3) / 2, dy * 2 * (1 + i)};
}

// =============================================================================

Board::Board(const ::runebound::map::MapClient &map) {
    for (int row = 0; row < ::runebound::map::STANDARD_SIZE; ++row) {
        for (int col = 0; col < ::runebound::map::STANDARD_SIZE; ++col) {
            auto center = centerize(row, col);

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
                auto [special_type_cell_key, special_fill_color] =
                    *SPECIAL_COLOR.find(special);
                add_special(
                    {center + Point<int>(HEXAGON_RADIUS / 2, 0),
                     HEXAGON_RADIUS / 5},
                    special_fill_color, {0x00, 0x00, 0x00, 0xFF}
                );
            }

            // add token if there is
            ::runebound::AdventureType token;
            if ((token = map.m_map[row][col].get_token()) !=
                ::runebound::AdventureType::NOTHING) {
                add_token(
                    {center.cast_to<short>(), HEXAGON_RADIUS / 2},
                    (*ADVENTURE_COLOR.find(token)).second,
                    {0x00, 0x00, 0x00, 0xFF}
                );
            }

            // adding roads if is it TODO
            if (map.m_map[row][col].check_road()) {
                for (auto [i, j] :
                     map.get_all_neighbours(::runebound::Point(row, col))) {
                    if (map.m_map[i][j].check_road()) {
                        Segment<short> seg(
                            center.cast_to<short>(),
                            centerize(i, j).cast_to<short>()
                        );
                        add_road(seg, {0x80, 0x80, 0x80, 0xFF});
                        m_is_connected_to_town.push_back(false);
                    }
                    if (map.m_map[i][j].get_type_cell() ==
                        ::runebound::map::TypeCell::TOWN) {
                        Segment<short> seg(
                            center.cast_to<short>(),
                            centerize(i, j).cast_to<short>()
                        );
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
        Segment<short> seg = {
            hex.get_vertex(v.first).cast_to<short>(),
            hex.get_vertex(v.second).cast_to<short>()};
        add_river(seg, river_color);
    }
}

void Board::add_cell(
    const HexagonShape &hex,
    SDL_Color fill_col,
    SDL_Color border_col
) {
    m_cells.push_back(hex);
    m_cell_fill_color.push_back(fill_col);
    m_cell_border_color.push_back(border_col);
    ++m_cell_amount;
}

void Board::add_river(const Segment<short> &seg, SDL_Color col) {
    m_rivers.push_back(seg);
    m_river_color.push_back(col);
    ++m_river_amount;
}

void Board::add_road(const Segment<short> &seg, SDL_Color col) {
    m_roads.push_back(seg);
    m_road_color.push_back(col);
    ++m_road_amount;
}

void Board::add_token(
    const CircleShape &cir,
    SDL_Color fill_col,
    SDL_Color border_col
) {
    m_tokens.push_back(cir);
    m_token_fill_color.push_back(fill_col);
    m_token_border_color.push_back(border_col);
    ++m_token_amount;
}

void Board::add_special(
    const SquareShape &sqr,
    SDL_Color fill_col,
    SDL_Color border_col
) {
    m_specials.push_back(sqr);
    m_special_fill_color.push_back(fill_col);
    m_special_border_color.push_back(border_col);
    ++m_special_amount;
}

void Board::render(SDL_Renderer *renderer) const {
    // cells
    for (::std::size_t i = 0; i < m_cell_amount; ++i) {
        m_cells[i].render(
            renderer, m_cell_fill_color[i], m_cell_border_color[i]
        );
    }
    if (m_selected_cell != 0xFFFF && m_selected_token == 0xFFFF) {
        m_cells[m_selected_cell].render(
            renderer, SELECTED_COLOR, m_cell_border_color[m_selected_cell]
        );
    }

    // rivers TODO
    for (::std::size_t i = 0; i < m_river_amount; ++i) {
        m_rivers[i].render(renderer, m_river_color[i], 5);
    }
    for (::std::size_t i = 0; i < m_road_amount; ++i) {
        if (m_is_connected_to_town[i]) {
            m_roads[i].half_render(renderer, m_road_color[i], short(7));
        } else {
            m_roads[i].render(renderer, m_road_color[i], short(7));
        }
    }

    // specials
    for (::std::size_t i = 0; i < m_special_amount; ++i) {
        m_specials[i].render(
            renderer, m_special_fill_color[i], m_special_border_color[i]
        );
    }

    // tokens
    for (::std::size_t i = 0; i < m_token_amount; ++i) {
        m_tokens[i].render(
            renderer, m_token_fill_color[i], m_token_border_color[i]
        );
    }
    if (m_selected_token != 0xFFFF) {
        m_tokens[m_selected_token].render(
            renderer, SELECTED_COLOR, m_token_border_color[m_selected_token]
        );
    }
}

void Board::update_selection(const Point<int> &dot) {
    m_selected_cell = 0xFFFF;
    m_selected_token = 0xFFFF;
    for (::std::size_t i = 0; i < m_cell_amount; ++i) {
        if (m_cells[i].in_bounds(dot)) {
            m_selected_cell = i;
            break;
        }
    }
    for (::std::size_t i = 0; i < m_token_amount; ++i) {
        if (m_tokens[i].in_bounds(dot.cast_to<short>())) {
            m_selected_token = i;
            break;
        }
    }
}
}  // namespace runebound::graphics