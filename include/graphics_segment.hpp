#ifndef RUNEBOUND_GRAPHICS_SEGMENT_HPP_
#define RUNEBOUND_GRAPHICS_SEGMENT_HPP_

#include <SDL2/SDL.h>
#include <graphics_point.hpp>

namespace runebound::graphics {
class Segment {
private:
    Point m_s, m_f;

public:
    Segment() = default;

    Segment(const Point &s, const Point &f) : m_s(s), m_f(f){};

    void render(SDL_Renderer *renderer, SDL_Color col, int size) const;

    // TODO
    void half_render(SDL_Renderer *renderer, SDL_Color col, int size) const;

    [[nodiscard]] Point s() const {
        return m_s;
    };

    [[nodiscard]] Point f() const {
        return m_f;
    };
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_SEGMENT_HPP_