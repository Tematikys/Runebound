#ifndef RUNEBOUND_GRAPHICS_HPP_
#define RUNEBOUND_GRAPHICS_HPP_

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
    explicit HexagonShape(Point center, int radius);
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_HPP_