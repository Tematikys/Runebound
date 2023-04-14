#ifndef POINT_HPP_
#define POINT_HPP_
#include <nlohmann/json.hpp>

namespace runebound {

void to_json(nlohmann::json &json, const Point &point);
void from_json(const nlohmann::json &json, Point &point);

struct Point {
public:
    int x, y;

    Point() = default;

    Point(int x_, int y_) : x(x_), y(y_) {
    }

    bool operator<(const Point &point) const {
        return (x < point.x) || (x == point.x && y < point.y);
    }

    bool operator==(const Point &point) const {
        return x == point.x && y == point.y;
    }

    friend void to_json(nlohmann::json &json, const Point &point) {
        json["x"] = point.x;
        json["y"] = point.y;
    }

    friend void from_json(const nlohmann::json &json, Point &point) {
        point.x = json["x"];
        point.y = json["y"];
    }

    friend Point operator+(const Point &lhs, const Point &rhs) {
        return Point(lhs.x + rhs.x, lhs.y + rhs.y);
    }
};
}  // namespace runebound
#endif  // POINT_HPP_
