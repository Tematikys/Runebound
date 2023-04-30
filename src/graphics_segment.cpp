#include <SDL2/SDL2_gfxPrimitives.h>
#include <graphics_segment.hpp>

namespace runebound::graphics {
void Segment::render(SDL_Renderer *renderer, SDL_Color color, int size) const {
    thickLineRGBA(
        renderer, static_cast<int16_t>(m_start.x()),
        static_cast<int16_t>(m_start.y()), static_cast<int16_t>(m_finish.x()),
        static_cast<int16_t>(m_finish.y()), 2 * size + 1, color.r, color.g,
        color.b, color.a
    );
    filledCircleRGBA(
        renderer, static_cast<int16_t>(m_start.x()),
        static_cast<int16_t>(m_start.y()), static_cast<int16_t>(size), color.r,
        color.g, color.b, color.a
    );
    filledCircleRGBA(
        renderer, static_cast<int16_t>(m_finish.x()),
        static_cast<int16_t>(m_finish.y()), static_cast<int16_t>(size), color.r,
        color.g, color.b, color.a
    );
}

// TODO
void Segment::half_render(SDL_Renderer *renderer, SDL_Color color, int size)
    const {
    thickLineRGBA(
        renderer, static_cast<int16_t>(m_start.x()),
        static_cast<int16_t>(m_start.y()),
        static_cast<int16_t>((m_start.x() + m_finish.x()) / 2),
        static_cast<int16_t>((m_start.y() + m_finish.y()) / 2), 2 * size + 1,
        color.r, color.g, color.b, color.a
    );
    filledCircleRGBA(
        renderer, static_cast<int16_t>(m_start.x()),
        static_cast<int16_t>(m_start.y()), static_cast<int16_t>(size), color.r,
        color.g, color.b, color.a
    );
}
}  // namespace runebound::graphics