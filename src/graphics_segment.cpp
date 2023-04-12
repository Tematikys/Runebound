#include <SDL2/SDL2_gfxPrimitives.h>
#include <graphics_segment.hpp>

namespace runebound::graphics {
void Segment::render(SDL_Renderer *renderer, SDL_Color color, int size) const {
    thickLineRGBA(
        renderer, m_start.x(), m_start.y(), m_finish.x(), m_finish.y(),
        2 * size + 1, color.r, color.g, color.b, color.a
    );
    filledCircleRGBA(
        renderer, m_start.x(), m_start.y(), size, color.r, color.g, color.b,
        color.a
    );
    filledCircleRGBA(
        renderer, m_finish.x(), m_finish.y(), size, color.r, color.g, color.b,
        color.a
    );
}

// TODO
void Segment::half_render(SDL_Renderer *renderer, SDL_Color color, int size)
    const {
    thickLineRGBA(
        renderer, m_start.x(), m_start.y(), ((m_start.x() + m_finish.x()) / 2),
        ((m_start.y() + m_finish.y()) / 2), 2 * size + 1, color.r, color.g,
        color.b, color.a
    );
    filledCircleRGBA(
        renderer, m_start.x(), m_start.y(), size, color.r, color.g, color.b,
        color.a
    );
}
}  // namespace runebound::graphics