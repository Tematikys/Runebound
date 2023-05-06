#ifndef RUNEBOUND_GRAPHICS_SHAPES_HPP_
#define RUNEBOUND_GRAPHICS_SHAPES_HPP_
#include <SDL2/SDL.h>
#include <graphics_point.hpp>
#include <tuple>
#include <utility>
#include <vector>

namespace runebound::graphics {
class PolygonShape {
protected:
    ::std::vector<Point> m_vertexes{};
    ::std::vector<::std::tuple<int, int, int>> m_side_coefficients{};

public:
    PolygonShape() = default;

    explicit PolygonShape(const ::std::vector<Point> &vertexes)
        : m_vertexes(vertexes) {
        init_side_coefficients();
    }

    void init_side_coefficients();

    void render(
        SDL_Renderer *renderer,
        int x_offset,
        int y_offset,
        SDL_Color fill_color
    ) const;

    void render_border(
        SDL_Renderer *renderer,
        int x_offset,
        int y_offset,
        SDL_Color border_color
    ) const;

    void render_to_texture(
        SDL_Renderer *renderer,
        SDL_Texture *&texture,
        SDL_Color fill_color,
        SDL_Color border_color
    ) const;

    void render_border_to_texture(
        SDL_Renderer *renderer,
        SDL_Texture *previous_texture,
        SDL_Texture *texture_to_render_on,
        int x_offset,
        int y_offset,
        SDL_Color border_color
    ) const;

    [[nodiscard]] bool in_bounds(const Point &dot) const;

    [[nodiscard]] Point get_vertex(::std::size_t index) const {
        return m_vertexes[index];
    };

    [[nodiscard]] ::std::size_t get_number_of_vertexes() const {
        return m_vertexes.size();
    };
};

class HexagonShape : public PolygonShape {
public:
    HexagonShape() = default;

    HexagonShape(const Point &center, int radius);
};

class SquareShape : public PolygonShape {
public:
    SquareShape() = default;

    SquareShape(const Point &center, int radius);
};

class RectangleShape : public PolygonShape {
public:
    RectangleShape() = default;

    RectangleShape(int x, int y, int width, int height);
};

class CircleShape {
private:
    Point m_center;
    int m_radius = 0;

public:
    CircleShape() = default;

    CircleShape(const Point &center, int radius)
        : m_center(center), m_radius(radius){};

    void render(
        SDL_Renderer *renderer,
        SDL_Color fill_color,
        int x_offset,
        int y_offset
    ) const;

    void render_border(
        SDL_Renderer *renderer,
        SDL_Color border_color,
        int x_offset,
        int y_offset
    ) const;

    [[nodiscard]] bool in_bounds(const Point &dot) const;
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_SHAPES_HPP_
