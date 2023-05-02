#ifndef RUNEBOUND_GRAPHICS_BOARD_HPP_
#define RUNEBOUND_GRAPHICS_BOARD_HPP_

#include <SDL2/SDL.h>
#include <graphics.hpp>
#include <graphics_config.hpp>
#include <graphics_point.hpp>
#include <graphics_segment.hpp>
#include <graphics_shapes.hpp>
#include <map_client.hpp>
#include <optional>
#include <vector>

namespace runebound::graphics {
class Board {
private:
    ::std::vector<HexagonShape> m_cells{};
    ::std::vector<SDL_Color> m_cell_fill_color{};
    ::std::vector<SDL_Color> m_cell_border_color{};
    ::std::size_t m_cell_amount{0};
    ::std::size_t m_selected_cell{0xFFFF};

    ::std::vector<CircleShape> m_tokens{};
    ::std::vector<SDL_Color> m_token_fill_color{};
    ::std::vector<SDL_Color> m_token_border_color{};
    ::std::size_t m_token_amount{0};
    ::std::size_t m_selected_token{0xFFFF};

    ::std::vector<SquareShape> m_specials{};
    ::std::vector<SDL_Color> m_special_fill_color{};
    ::std::vector<SDL_Color> m_special_border_color{};
    ::std::size_t m_special_amount{0};

    ::std::vector<Segment> m_rivers{};
    ::std::vector<SDL_Color> m_river_color{};
    ::std::size_t m_river_amount{0};
    // TODO refactor
    ::std::vector<bool> m_is_connected_to_town = {};

    ::std::vector<Segment> m_roads{};
    ::std::vector<SDL_Color> m_road_color{};
    ::std::size_t m_road_amount{0};

public:
    Board() = default;

    explicit Board(const ::runebound::map::MapClient &map);

    void add_cell(
        const HexagonShape &hexagon,
        SDL_Color fill_color,
        SDL_Color border_color
    );

    void add_token(
        const CircleShape &circle,
        SDL_Color fill_color,
        SDL_Color border_color
    );

    void add_special(
        const SquareShape &square,
        SDL_Color fill_color,
        SDL_Color border_color
    );

    void add_river(const Segment &segment, SDL_Color color);

    void add_road(const Segment &segment, SDL_Color color);

    void render(SDL_Renderer *renderer) const;

    void update_selection(const Point &dot);

    [[nodiscard]] ::std::size_t get_selected_hexagon() const {
        return m_selected_cell;
    }
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_BOARD_HPP_
