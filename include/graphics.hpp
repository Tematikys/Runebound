#ifndef RUNEBOUND_GRAPHICS_HPP_
#define RUNEBOUND_GRAPHICS_HPP_

#include <SDL2/SDL.h>
#include <map.hpp>
#include <map_cell.hpp>
#include <optional>
#include <tuple>
#include <vector>

namespace runebound::graphics {

// basic point class
class Point {
private:
    int m_x, m_y;

public:
    explicit Point() : m_x(), m_y(){};

    explicit Point(int x, int y) : m_x(x), m_y(y){};

    [[nodiscard]] int x() const {
        return m_x;
    };

    [[nodiscard]] int y() const {
        return m_y;
    };
};

// basic polygon class
class PolygonShape {
protected:
    ::std::vector<Point> m_vertexes;
    ::std::vector<std::tuple<int, int, int>> m_side_coefficients;

public:
    PolygonShape() = default;

    explicit PolygonShape(::std::vector<Point> &vertexes)
        : m_vertexes(::std::move(vertexes)) {
        init_side_coefficients();
    }

    void init_side_coefficients();

    // check is dot is inside the polygon
    [[nodiscard]] bool in_bounds(Point dot) const;

    void render(
        SDL_Renderer *renderer,
        SDL_Color fill_color,
        SDL_Color border_color
    ) const;

    [[nodiscard]] Point get_vertex(::std::size_t index) const {
        return m_vertexes[index];
    }

    [[nodiscard]] ::std::vector<Point> get_vertexes() const {
        return m_vertexes;
    };

    [[nodiscard]] ::std::size_t get_number_of_vertexes() const {
        return m_vertexes.size();
    };
};

// basic hexagon class, derived to polygon
class HexagonShape : public PolygonShape {
public:
    explicit HexagonShape() = default;

    explicit HexagonShape(Point center, int radius);
};

// board class, contains hexagons, has connected methods
class Board {
private:
    ::std::vector<HexagonShape> m_hexagons;
    ::std::vector<SDL_Color> m_fill_colors;
    ::std::vector<SDL_Color> m_border_color;
    ::std::size_t m_hexagon_amount{0};

public:
    explicit Board() = default;

    explicit Board(
        ::std::vector<HexagonShape> &hexagons,
        ::std::vector<SDL_Color> fill_colors,
        ::std::vector<SDL_Color> border_colors
    )
        : m_hexagons(::std::move(hexagons)),
          m_fill_colors(::std::move(fill_colors)),
          m_border_color(::std::move(border_colors)),
          m_hexagon_amount(m_hexagons.size()){};

    void add_hexagon(
        HexagonShape &hex,
        SDL_Color fill_color,
        SDL_Color border_color
    );

    void render(SDL_Renderer *renderer) const;

    ::std::optional<::std::size_t> in_bounds(Point dot);
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_HPP_