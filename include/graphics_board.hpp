#ifndef RUNEBOUND_GRAPHICS_BOARD_HPP_
#define RUNEBOUND_GRAPHICS_BOARD_HPP_

#include <graphics.hpp>
#include <graphics_config.hpp>
#include <graphics_point.hpp>
#include <graphics_segment.hpp>
#include <graphics_shapes.hpp>
#include <graphics_texture.hpp>
#include <map_client.hpp>
#include <utility>
#include <vector>

namespace runebound::graphics {
class Board {
private:
    std::vector<HexagonShape> m_cells{};
    std::vector<SDL_Color> m_cell_fill_color{};
    std::vector<SDL_Color> m_cell_border_color{};
    std::size_t m_cell_amount{0};
    std::size_t m_selected_cell{0xFFFF};

    std::vector<CircleShape> m_tokens{};
    std::vector<SDL_Color> m_token_fill_color{};
    std::vector<SDL_Color> m_token_border_color{};
    std::size_t m_token_amount{0};
    std::size_t m_selected_token{0xFFFF};

    std::vector<std::string> m_specials{};
    std::vector<Point> m_specials_pos{};
    std::size_t m_special_amount{0};

    std::vector<Segment> m_rivers{};
    std::vector<SDL_Color> m_river_color{};
    std::size_t m_river_amount{0};
    std::vector<bool> m_is_connected_to_town{};

    std::vector<Segment> m_roads{};
    std::vector<SDL_Color> m_road_color{};
    std::size_t m_road_amount{0};

    std::vector<Point> m_available_hexagons{};

    int m_width{0};
    int m_height{0};

    void add_rivers(const ::runebound::map::MapClient &map);

    void add_river(const Segment &segment, SDL_Color color);

    void add_cell(const ::runebound::map::MapClient &map, int row, int col);

    void add_token(const ::runebound::map::MapClient &map, int row, int col);

    void add_special(const ::runebound::map::MapClient &map, int row, int col);

    void add_road(const ::runebound::map::MapClient &map, int row, int col);

public:
    Board() = default;

    explicit Board(const ::runebound::map::MapClient &map);

    void render(
        SDL_Renderer *renderer,
        int x_offset,
        int y_offset,
        std::map<std::string, Texture> &images
    ) const;

    void render_to_texture(
        SDL_Renderer *renderer,
        SDL_Texture *&texture,
        std::map<std::string, Texture> &images
    ) const;

    void update_selection(const Point &dot);

    void update_available_hexagons(std::vector<Point> hexagons) {
        m_available_hexagons = std::move(hexagons);
    }

    [[nodiscard]] int width() const {
        return m_width;
    }

    [[nodiscard]] int height() const {
        return m_height;
    }

    [[nodiscard]] std::size_t get_selected_hexagon() const {
        return m_selected_cell;
    }
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_BOARD_HPP_
