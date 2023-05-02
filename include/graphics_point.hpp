#ifndef RUNEBOUND_GRAPHICS_POINT_HPP_
#define RUNEBOUND_GRAPHICS_POINT_HPP_
#include <utility>

namespace runebound::graphics {
class Point;
Point operator+(const Point &, const Point &);

class Point {
private:
    int m_x{0};
    int m_y{0};

public:
    Point() = default;

    Point(int x, int y) : m_x(x), m_y(y){};

    explicit Point(::std::pair<int, int> coords)
        : m_x(coords.first), m_y(coords.second){};

    [[nodiscard]] int x() const {
        return m_x;
    };

    [[nodiscard]] int y() const {
        return m_y;
    };

    friend Point operator+(const Point &lhs, const Point &rhs);
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_POINT_HPP_