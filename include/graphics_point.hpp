#ifndef RUNEBOUND_GRAPHICS_POINT_HPP_
#define RUNEBOUND_GRAPHICS_POINT_HPP_

#include <utility>

namespace runebound::graphics {
template <typename T>
class Point;

template <typename U>
Point<U> operator+(const Point<U> &lhs, const Point<U> &rhs);

// basic point class
template <typename T>
class Point {
private:
    T m_x, m_y;

public:
    Point() : m_x(), m_y(){};

    Point(T x, T y) : m_x(x), m_y(y){};

    Point(const Point<T> &other) : m_x(other.x()), m_y(other.y()){};

    explicit Point(::std::pair<T, T> coords)
        : m_x(coords.first), m_y(coords.second){};

    template <typename U>
    Point<U> cast_to() const {
        return Point<U>(static_cast<U>(m_x), static_cast<U>(m_y));
    }

    [[nodiscard]] T x() const {
        return m_x;
    };

    [[nodiscard]] T y() const {
        return m_y;
    };

    //    Point<T> operator+(const Point<T> &other) const {
    //        return {x() + other.x(), y() + other.y()};
    //    }

    template <typename U>
    friend Point<U> operator+(const Point<U> &lhs, const Point<U> &rhs);
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_POINT_HPP_