#include <SDL2/SDL2_gfxPrimitives.h>
#include <graphics_segment.hpp>

void runebound::graphics::Segment::render(SDL_Renderer *renderer, SDL_Color col)
    const {
    thickLineRGBA(
        renderer, m_s.x(), m_s.y(), m_f.x(), m_f.y(), 11, col.r, col.g, col.b,
        col.a
    );
    filledCircleRGBA(renderer, m_s.x(), m_s.y(), 5, col.r, col.g, col.b, col.a);
    filledCircleRGBA(renderer, m_f.x(), m_f.y(), 5, col.r, col.g, col.b, col.a);
}
