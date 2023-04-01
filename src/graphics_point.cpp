#include <graphics_point.hpp>

namespace runebound::graphics {
template <typename U>
Point<U> operator+(const Point<U> &lhs, const Point<U> &rhs) {
    return {lhs.x() + rhs.x(), lhs.y() + rhs.y()};
}

template Point<int> operator+(const Point<int> &, const Point<int> &);
}  // namespace runebound::graphics