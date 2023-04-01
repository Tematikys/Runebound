#ifndef RUNEBOUND_GRAPHICS_SEGMENT_HPP_
#define RUNEBOUND_GRAPHICS_SEGMENT_HPP_

#include <SDL2/SDL.h>
#include <graphics_point.hpp>

namespace runebound::graphics {
template <typename T>
class Segment {
private:
    Point<T> m_s, m_f;

public:
    Segment() = default;

    Segment(const Point<T> &s, const Point<T> &f) : m_s(s), m_f(f){};

    template <typename U>
    Point<U> cast_to() const {
        return Point<U>(static_cast<U>(m_s), static_cast<U>(m_f));
    }

    void render(SDL_Renderer *renderer, SDL_Color col, T size) const;

    void half_render(SDL_Renderer *renderer, SDL_Color col, T size) const;

    [[nodiscard]] Point<T> s() const {
        return m_s;
    };

    [[nodiscard]] Point<T> f() const {
        return m_f;
    };
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_SEGMENT_HPP_