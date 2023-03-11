#ifndef RUNEBOUND_GRAPHICS_HPP_
#define RUNEBOUND_GRAPHICS_HPP_

#include <vector>

namespace runebound::graphics {
class Point {
public:
    int x, y;

    Point() : x(), y(){};

    Point(int x_, int y_) : x(x_), y(y_){};
};

class PolygonShape {
private:
    std::vector<Point> vertices;
    Point center;
    std::size_t length;

public:
    explicit PolygonShape(const std::vector<Point> &vertices_);

    [[nodiscard]] Point getCenter() const {
        return center;
    };

    [[nodiscard]] std::vector<Point> getVertices() const {
        return vertices;
    };

    [[nodiscard]] std::size_t getNumberOfVertices() const {
        return length;
    };
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_HPP_