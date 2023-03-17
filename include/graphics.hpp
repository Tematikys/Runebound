#ifndef RUNEBOUND_GRAPHICS_HPP_
#define RUNEBOUND_GRAPHICS_HPP_

#include <SDL2/SDL.h>
#include <map.hpp>
#include <map_cell.hpp>
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

public:
    PolygonShape() = default;

    explicit PolygonShape(::std::vector<Point> &vertexes)
        : m_vertexes(::std::move(vertexes)){};

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
private:
    std::vector<std::tuple<int, int, int>> m_side_coefficients;

public:
    explicit HexagonShape() = default;

    explicit HexagonShape(Point center, int radius);

    [[nodiscard]] bool in_bounds(Point dot) const;
};

// draw colored polygon function declaration
bool draw_filled_polygon(
    const PolygonShape &polygon,
    SDL_Color color,
    SDL_Renderer *renderer
);

// draw polygon border function declaration
bool draw_polygon_border(
    const PolygonShape &polygon,
    SDL_Color color,
    SDL_Renderer *renderer
);

// draw map function declaration
bool draw_map(const ::runebound::map::Map &map, SDL_Renderer *renderer);
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_HPP_