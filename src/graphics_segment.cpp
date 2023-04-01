#include <SDL2/SDL2_gfxPrimitives.h>
#include <graphics_segment.hpp>

namespace runebound::graphics {
template <typename T>
void Segment<T>::render(SDL_Renderer *renderer, SDL_Color col, T size) const {
    thickLineRGBA(
        renderer, m_s.x(), m_s.y(), m_f.x(), m_f.y(), 2 * size + 1, col.r,
        col.g, col.b, col.a
    );
    filledCircleRGBA(
        renderer, m_s.x(), m_s.y(), size, col.r, col.g, col.b, col.a
    );
    filledCircleRGBA(
        renderer, m_f.x(), m_f.y(), size, col.r, col.g, col.b, col.a
    );
}

// TODO
template <typename T>
void Segment<T>::half_render(SDL_Renderer *renderer, SDL_Color col, T size)
    const {
    thickLineRGBA(
        renderer, m_s.x(), m_s.y(), (m_s.x() + m_f.x()) / 2,
        (m_s.y() + m_f.y()) / 2, 2 * size + 1, col.r, col.g, col.b, col.a
    );
    filledCircleRGBA(
        renderer, m_s.x(), m_s.y(), size, col.r, col.g, col.b, col.a
    );
}

template class Segment<int>;
template class Segment<short>;
}  // namespace runebound::graphics