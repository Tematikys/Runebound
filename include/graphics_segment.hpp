#ifndef RUNEBOUND_GRAPHICS_SEGMENT_HPP_
#define RUNEBOUND_GRAPHICS_SEGMENT_HPP_

#include <graphics_point.hpp>

namespace runebound::graphics {
class Segment {
private:
    Point m_s, m_f;

public:
    Segment() : m_s(), m_f(){};

    Segment(Point s, Point f) : m_s(s), m_f(f){};

    [[nodiscard]] Point s() const {
        return m_s;
    };

    [[nodiscard]] Point f() const {
        return m_f;
    };
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_SEGMENT_HPP_