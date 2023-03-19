#ifndef RUNEBOUND_GRAPHICS_SHAPES_HPP_
#define RUNEBOUND_GRAPHICS_SHAPES_HPP_

#include <SDL2/SDL.h>
#include <graphics_point.hpp>
#include <tuple>
#include <vector>

namespace runebound::graphics {
// basic polygon class
class PolygonShape {
protected:
    ::std::vector<Point> m_vertexes;
    ::std::vector<::std::tuple<int, int, int>> m_side_coefficients;

public:
    explicit PolygonShape() = default;

    [[maybe_unused]] explicit PolygonShape(::std::vector<Point> &vertexes)
        : m_vertexes(::std::move(vertexes)) {
        init_side_coefficients();
    }

    void init_side_coefficients();

    void render(
        SDL_Renderer *renderer,
        SDL_Color fill_color,
        SDL_Color border_color
    ) const;

    [[nodiscard]] bool in_bounds(Point dot) const;

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
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_SHAPES_HPP_
