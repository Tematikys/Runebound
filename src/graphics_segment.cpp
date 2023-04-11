#include <SDL2/SDL2_gfxPrimitives.h>
#include <graphics_segment.hpp>

namespace runebound::graphics {
void Segment::render(SDL_Renderer *renderer, SDL_Color col, int size) const {
    thickLineRGBA(
        renderer, (short)m_s.x(), (short)m_s.y(), (short)m_f.x(),
        (short)m_f.y(), 2 * size + 1, col.r, col.g, col.b, col.a
    );
    filledCircleRGBA(
        renderer, (short)m_s.x(), (short)m_s.y(), (short)size, col.r, col.g,
        col.b, col.a
    );
    filledCircleRGBA(
        renderer, (short)m_f.x(), (short)m_f.y(), (short)size, col.r, col.g,
        col.b, col.a
    );
}

// TODO
void Segment::half_render(SDL_Renderer *renderer, SDL_Color col, int size)
    const {
    thickLineRGBA(
        renderer, (short)m_s.x(), (short)m_s.y(),
        (short)((m_s.x() + m_f.x()) / 2), (short)((m_s.y() + m_f.y()) / 2),
        2 * size + 1, col.r, col.g, col.b, col.a
    );
    filledCircleRGBA(
        renderer, (short)m_s.x(), (short)m_s.y(), (short)size, col.r, col.g,
        col.b, col.a
    );
}
}  // namespace runebound::graphics