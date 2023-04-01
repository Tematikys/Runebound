#ifndef RUNEBOUND_GRAPHICS_SHAPES_HPP_
#define RUNEBOUND_GRAPHICS_SHAPES_HPP_

#include <SDL2/SDL.h>
#include <graphics_point.hpp>
#include <tuple>
#include <utility>
#include <vector>

namespace runebound::graphics {
// basic polygon class
class PolygonShape {
protected:
    ::std::vector<Point<int>> m_vertexes;
    ::std::vector<::std::tuple<int, int, int>> m_side_coefficients;

public:
    PolygonShape() = default;

    explicit PolygonShape(::std::vector<Point<int>> vertexes) : m_vertexes(std::move(vertexes)) {
        init_side_coefficients();
    }

    void init_side_coefficients();

    void render(
        SDL_Renderer *renderer,
        SDL_Color fill_color,
        SDL_Color border_color
    ) const;

    [[nodiscard]] bool in_bounds(Point<int> dot) const;

    [[nodiscard]] Point<int> get_vertex(::std::size_t index) const {
        return m_vertexes[index];
    };

    [[nodiscard]] ::std::size_t get_number_of_vertexes() const {
        return m_vertexes.size();
    };
};

// basic hexagon class, derived to polygon
class HexagonShape : public PolygonShape {
public:
    HexagonShape() = default;

    HexagonShape(const Point<int> &center, int radius);
};

class SquareShape : public PolygonShape {
public:
    SquareShape() = default;

    SquareShape(const Point<int> & center, int radius);
};

// basic circle class
class CircleShape {
private:
    Point<Sint16> m_center;
    Sint16 m_radius{0};

public:
    CircleShape() = default;

    CircleShape(const Point<Sint16> &center, Sint16 radius)
        : m_center(center), m_radius(radius){};

    void render(
        SDL_Renderer *renderer,
        SDL_Color fill_color,
        SDL_Color border_color
    ) const;

    [[nodiscard]] bool in_bounds(const Point<Sint16> &dot) const;
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_SHAPES_HPP_
