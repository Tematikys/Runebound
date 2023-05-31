#include <SDL2/SDL2_gfxPrimitives.h>
#include <algorithm>
#include <graphics_shapes.hpp>

namespace runebound::graphics {
void PolygonShape::init_side_coefficients() {
    for (::std::size_t i = 0; i < m_vertexes.size(); ++i) {
        const int x1 = m_vertexes[(i + 1) % 6].x();
        const int y1 = m_vertexes[(i + 1) % 6].y();
        const int x2 = m_vertexes[i].x();
        const int y2 = m_vertexes[i].y();
        m_side_coefficients.emplace_back(y1 - y2, x2 - x1, x1 * y2 - x2 * y1);
    }
}

void PolygonShape::render(
    SDL_Renderer *renderer,
    int x_offset,
    int y_offset,
    SDL_Color fill_color
) const {
    SDL_SetRenderDrawColor(
        renderer, fill_color.r, fill_color.g, fill_color.b, fill_color.a
    );
    const int num_vertexes = static_cast<int>(get_number_of_vertexes());
    int num_vertexes_processed = 1;
    int top_y = m_vertexes[0].y();
    int top_index = 0;
    for (int i = 1; i < num_vertexes; ++i) {
        if (m_vertexes[i].y() < top_y) {
            top_y = m_vertexes[i].y();
            top_index = i;
        }
    }
    int left_index = top_index - 1;
    if (left_index < 0) {
        left_index = num_vertexes - 1;
    }
    int right_index = top_index + 1;
    if (right_index >= num_vertexes) {
        right_index = 0;
    }
    int left_x{};
    int right_x{};
    left_x = right_x = (m_vertexes[top_index].x()) << 16;
    int left_slope = 0;
    if (m_vertexes[left_index].y() != m_vertexes[top_index].y()) {
        left_slope =
            ((m_vertexes[left_index].x() - m_vertexes[top_index].x()) << 16) /
            (m_vertexes[left_index].y() - m_vertexes[top_index].y());
    }
    int right_slope = 0;
    if (m_vertexes[right_index].y() != m_vertexes[top_index].y()) {
        right_slope =
            ((m_vertexes[right_index].x() - m_vertexes[top_index].x()) << 16) /
            (m_vertexes[right_index].y() - m_vertexes[top_index].y());
    }
    int y = m_vertexes[top_index].y();
    while (num_vertexes_processed < num_vertexes) {
        while (y < m_vertexes[left_index].y() && y < m_vertexes[right_index].y()
        ) {
            SDL_RenderDrawLine(
                renderer, (left_x >> 16) + x_offset, y + y_offset,
                (right_x >> 16) + x_offset, y + y_offset
            );
            ++y;
            left_x += left_slope;
            right_x += right_slope;
        }
        if (m_vertexes[left_index].y() <= y) {
            top_index = left_index;
            --left_index;
            if (left_index < 0) {
                left_index = num_vertexes - 1;
            }
            if (m_vertexes[left_index].y() != m_vertexes[top_index].y()) {
                left_slope =
                    ((m_vertexes[left_index].x() - m_vertexes[top_index].x())
                     << 16) /
                    (m_vertexes[left_index].y() - m_vertexes[top_index].y());
            }
            left_x = (m_vertexes[top_index].x()) << 16;
            ++num_vertexes_processed;
        }
        if (m_vertexes[right_index].y() <= y) {
            top_index = right_index;
            ++right_index;
            if (right_index == num_vertexes) {
                right_index = 0;
            }
            if (m_vertexes[right_index].y() != m_vertexes[top_index].y()) {
                right_slope =
                    ((m_vertexes[right_index].x() - m_vertexes[top_index].x())
                     << 16) /
                    (m_vertexes[right_index].y() - m_vertexes[top_index].y());
            }
            right_x = (m_vertexes[top_index].x()) << 16;
            ++num_vertexes_processed;
        }
        SDL_RenderDrawLine(
            renderer, (left_x >> 16) + x_offset, y + y_offset,
            (right_x >> 16) + x_offset, y + y_offset
        );
    }
}

void PolygonShape::render_border(
    SDL_Renderer *renderer,
    int x_offset,
    int y_offset,
    SDL_Color border_color,
    int thickness
) const {
    for (::std::size_t i = 0; i < get_number_of_vertexes(); ++i) {
        circleRGBA(
            renderer, static_cast<short>(get_vertex(i).x() + x_offset),
            static_cast<short>(get_vertex(i).y() + y_offset),
            static_cast<short>(thickness / 2), border_color.r, border_color.g,
            border_color.b, border_color.a
        );
        thickLineRGBA(
            renderer, static_cast<short>(get_vertex(i).x() + x_offset),
            static_cast<short>(get_vertex(i).y() + y_offset),
            static_cast<short>(
                get_vertex((i + 1) % get_number_of_vertexes()).x() + x_offset
            ),
            static_cast<short>(
                get_vertex((i + 1) % get_number_of_vertexes()).y() + y_offset
            ),
            thickness, border_color.r, border_color.g, border_color.b,
            border_color.a
        );
    }
}

void PolygonShape::render_to_texture(
    SDL_Renderer *renderer,
    SDL_Texture *&texture,
    SDL_Color fill_color,
    SDL_Color border_color
) const {
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    //    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    render(renderer, 0, 0, fill_color);
    render_border(renderer, 0, 0, border_color, 1);
    //    SDL_SetRenderTarget(renderer, nullptr);
    //    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
}

void PolygonShape::render_border_to_texture(
    SDL_Renderer *renderer,
    SDL_Texture *previous_texture,
    SDL_Texture *texture_to_render_on,
    int x_offset,
    int y_offset,
    SDL_Color border_color
) const {
    SDL_SetRenderTarget(renderer, texture_to_render_on);
    render_border(renderer, x_offset, y_offset, border_color, 1);
    SDL_SetRenderTarget(renderer, previous_texture);
}

bool PolygonShape::in_bounds(const Point &dot) const {
    return ::std::all_of(
        m_side_coefficients.begin(), m_side_coefficients.end(),
        [&](::std::tuple<int, int, int> coefficients) {
            auto [a, b, c] = coefficients;
            return dot.x() * a + dot.y() * b + c <= 0;
        }
    );
}

HexagonShape::HexagonShape(const Point &center, int radius) {
    const int dy = (radius * 56756) >> 16;
    m_vertexes.emplace_back(center.x() - radius / 2, center.y() - dy);
    m_vertexes.emplace_back(center.x() + radius / 2, center.y() - dy);
    m_vertexes.emplace_back(center.x() + radius, center.y());
    m_vertexes.emplace_back(center.x() + radius / 2, center.y() + dy);
    m_vertexes.emplace_back(center.x() - radius / 2, center.y() + dy);
    m_vertexes.emplace_back(center.x() - radius, center.y());
    init_side_coefficients();
}

SquareShape::SquareShape(const Point &center, int radius) {
    m_vertexes.emplace_back(center + Point(-radius, -radius));
    m_vertexes.emplace_back(center + Point(radius, -radius));
    m_vertexes.emplace_back(center + Point(radius, radius));
    m_vertexes.emplace_back(center + Point(-radius, radius));
    init_side_coefficients();
}

RectangleShape::RectangleShape(int x, int y, int width, int height) {
    m_vertexes.emplace_back(x, y);
    m_vertexes.emplace_back(x + width, y);
    m_vertexes.emplace_back(x + width, y + height);
    m_vertexes.emplace_back(x, y + height);
    init_side_coefficients();
}

void CircleShape::render(
    SDL_Renderer *renderer,
    SDL_Color fill_color,
    int x_offset,
    int y_offset
) const {
    filledCircleRGBA(
        renderer, static_cast<int16_t>(m_center.x() + x_offset),
        static_cast<int16_t>(m_center.y() + y_offset),
        static_cast<int16_t>(m_radius), fill_color.r, fill_color.g,
        fill_color.b, fill_color.a
    );
}

void CircleShape::render_border(
    SDL_Renderer *renderer,
    SDL_Color border_color,
    int x_offset,
    int y_offset
) const {
    circleRGBA(
        renderer, static_cast<int16_t>(m_center.x() + x_offset),
        static_cast<int16_t>(m_center.y() + y_offset),
        static_cast<int16_t>(m_radius), border_color.r, border_color.g,
        border_color.b, border_color.a
    );
}

bool CircleShape::in_bounds(const Point &dot) const {
    const int dx = (m_center.x() - dot.x());
    const int dy = (m_center.y() - dot.y());
    return dx * dx + dy * dy < m_radius * m_radius;
}
}  // namespace runebound::graphics