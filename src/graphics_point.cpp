#include <graphics_point.hpp>

namespace runebound::graphics {
Point operator+(const Point &lhs, const Point &rhs) {
    return {lhs.x() + rhs.x(), lhs.y() + rhs.y()};
}
}  // namespace runebound::graphics