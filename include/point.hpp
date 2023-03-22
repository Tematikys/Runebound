#ifndef POINT_HPP_
#define POINT_HPP_
#include "json.hpp"

struct Point {
public:
    int x, y;

    Point() = default;

    Point (int x_, int y_) : x(x_), y(y_) {}

    bool operator <(const Point &point) const {
        return (x < point.x) || (x == point.x && y < point.y);
    }

    bool operator ==(const Point &point) const {
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
};
#endif  // POINT_HPP_
