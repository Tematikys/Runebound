#include <SDL2/SDL2_gfxPrimitives.h>
#include <graphics_segment.hpp>

namespace runebound::graphics {
void Segment::render(
    SDL_Renderer *renderer,
    SDL_Color color,
    int size,
    int x_offset,
    int y_offset
) const {
    thickLineRGBA(
        renderer, static_cast<int16_t>(m_start.x() + x_offset),
        static_cast<int16_t>(m_start.y() + y_offset),
        static_cast<int16_t>(m_finish.x() + x_offset),
        static_cast<int16_t>(m_finish.y() + y_offset), 2 * size + 1, color.r,
        color.g, color.b, color.a
    );
    filledCircleRGBA(
        renderer, static_cast<int16_t>(m_start.x() + x_offset),
        static_cast<int16_t>(m_start.y() + y_offset),
        static_cast<int16_t>(size), color.r, color.g, color.b, color.a
    );
    filledCircleRGBA(
        renderer, static_cast<int16_t>(m_finish.x() + x_offset),
        static_cast<int16_t>(m_finish.y() + y_offset),
        static_cast<int16_t>(size), color.r, color.g, color.b, color.a
    );
}

void Segment::half_render(
    SDL_Renderer *renderer,
    SDL_Color color,
    int size,
    int x_offset,
    int y_offset
) const {
    thickLineRGBA(
        renderer, static_cast<int16_t>(m_start.x() + x_offset),
        static_cast<int16_t>(m_start.y() + y_offset),
        static_cast<int16_t>(
            (m_start.x() + x_offset + m_finish.x() + x_offset) / 2
        ),
        static_cast<int16_t>(
            (m_start.y() + y_offset + m_finish.y() + y_offset) / 2
        ),
        2 * size + 1, color.r, color.g, color.b, color.a
    );
    filledCircleRGBA(
        renderer, static_cast<int16_t>(m_start.x() + x_offset),
        static_cast<int16_t>(m_start.y() + y_offset),
        static_cast<int16_t>(size), color.r, color.g, color.b, color.a
    );
}
}  // namespace runebound::graphics