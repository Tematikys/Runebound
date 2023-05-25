#ifndef RUNEBOUND_GRAPHICS_SEGMENT_HPP_
#define RUNEBOUND_GRAPHICS_SEGMENT_HPP_

#include <graphics_config.hpp>
#include <graphics_point.hpp>

namespace runebound::graphics {
class Segment {
private:
    Point m_start{};
    Point m_finish{};

public:
    Segment() = default;

    Segment(const Point &s, const Point &f) : m_start(s), m_finish(f){};

    void render(
        SDL_Renderer *renderer,
        SDL_Color color,
        int size,
        int x_offset,
        int y_offset
    ) const;

    void half_render(
        SDL_Renderer *renderer,
        SDL_Color color,
        int size,
        int x_offset,
        int y_offset
    ) const;

    [[nodiscard]] Point start() const {
        return m_start;
    };

    [[nodiscard]] Point finish() const {
        return m_finish;
    };
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_SEGMENT_HPP_