#include <graphics_segment.hpp>
#include <SDL2/SDL2_gfxPrimitives.h>

void runebound::graphics::Segment::render(
    SDL_Renderer *renderer,
    SDL_Color col
) const {
    thickLineRGBA(renderer, m_s.x(), m_s.y(), m_f.x(), m_f.y(), 5, col.r, col.g, col.b, col.a);
}
