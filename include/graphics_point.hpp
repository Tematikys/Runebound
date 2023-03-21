#ifndef RUNEBOUND_GRAPHICS_POINT_HPP_
#define RUNEBOUND_GRAPHICS_POINT_HPP_

namespace runebound::graphics {
// basic point class
class Point {
private:
    int m_x, m_y;

public:
    Point() : m_x(), m_y(){};
    Point(int x, int y) : m_x(x), m_y(y){};

    [[nodiscard]] int x() const {
        return m_x;
    };

    [[nodiscard]] int y() const {
        return m_y;
    };
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_POINT_HPP_