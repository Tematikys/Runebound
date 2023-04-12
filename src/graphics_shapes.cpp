#include <SDL2/SDL2_gfxPrimitives.h>
#include <algorithm>
#include <graphics_shapes.hpp>

namespace runebound::graphics {
// initialize polygon's sides coefficients
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
    SDL_Color fill_color,
    int x_offset,
    int y_offset
) const {
    SDL_SetRenderDrawColor(
        renderer, fill_color.r, fill_color.g, fill_color.b, fill_color.a
    );

    // get and set necessary variables
    const int num_vertexes = static_cast<int>(get_number_of_vertexes());
    int num_vertexes_processed = 1;

    // find the highest vertex
    int top_y = m_vertexes[0].y();
    int top_index = 0;
    for (int i = 1; i < num_vertexes; ++i) {
        if (m_vertexes[i].y() < top_y) {
            top_y = m_vertexes[i].y();
            top_index = i;
        }
    }

    // left-side vertex
    int left_index = top_index - 1;
    if (left_index < 0) {
        left_index = num_vertexes - 1;
    }
    // right-side vertex
    int right_index = top_index + 1;
    if (right_index >= num_vertexes) {
        right_index = 0;
    }

    // left, right-side x
    int left_x;
    int right_x;
    left_x = right_x = (m_vertexes[top_index].x()) << 16;

    // left dx slope
    int left_slope = 0;
    if (m_vertexes[left_index].y() != m_vertexes[top_index].y()) {
        left_slope =
            ((m_vertexes[left_index].x() - m_vertexes[top_index].x()) << 16) /
            (m_vertexes[left_index].y() - m_vertexes[top_index].y());
    }
    // right dx slope
    int right_slope = 0;
    if (m_vertexes[right_index].y() != m_vertexes[top_index].y()) {
        right_slope =
            ((m_vertexes[right_index].x() - m_vertexes[top_index].x()) << 16) /
            (m_vertexes[right_index].y() - m_vertexes[top_index].y());
    }

    // y coordinate
    int y = m_vertexes[top_index].y();

    // until all the vertices are drawn, draw
    while (num_vertexes_processed < num_vertexes) {
        // while y is above side m_vertexes, draw horizontal lines
        while (y < m_vertexes[left_index].y() && y < m_vertexes[right_index].y()
        ) {
            // draw line
            SDL_RenderDrawLine(
                renderer, (left_x >> 16) + x_offset, y + y_offset,
                (right_x >> 16) + x_offset, y + y_offset
            );
            // lower y
            ++y;
            // shift xs
            left_x += left_slope;
            right_x += right_slope;
        }

        // if y coordinate is on the same level as left vertex, go to next
        if (m_vertexes[left_index].y() <= y) {
            // make top vertex equal left
            top_index = left_index;
            // go to next vertex
            --left_index;
            // module
            if (left_index < 0) {
                left_index = num_vertexes - 1;
            }

            // change slope if needed
            if (m_vertexes[left_index].y() != m_vertexes[top_index].y()) {
                left_slope =
                    ((m_vertexes[left_index].x() - m_vertexes[top_index].x())
                     << 16) /
                    (m_vertexes[left_index].y() - m_vertexes[top_index].y());
            }

            // << 16 is used for rounding
            left_x = (m_vertexes[top_index].x()) << 16;
            // +1 processed vertex
            ++num_vertexes_processed;
        }

        // same for right side
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

        // draw connecting line
        SDL_RenderDrawLine(
            renderer, (left_x >> 16) + x_offset, y + y_offset,
            (right_x >> 16) + x_offset, y + y_offset
        );
    }
}

void PolygonShape::render_border(
    SDL_Renderer *renderer,
    SDL_Color border_color,
    int x_offset,
    int y_offset
) const {
    SDL_SetRenderDrawColor(
        renderer, border_color.r, border_color.g, border_color.b, border_color.a
    );

    // draw every edge of polygon except last
    for (::std::size_t i = 0; i < get_number_of_vertexes(); ++i) {
        SDL_RenderDrawLine(
            renderer, get_vertex(i).x() + x_offset,
            get_vertex(i).y() + y_offset,
            get_vertex((i + 1) % get_number_of_vertexes()).x() + x_offset,
            get_vertex((i + 1) % get_number_of_vertexes()).y() + y_offset
        );
    }
}

// check that point is inside of polygon
bool PolygonShape::in_bounds(const Point &dot) const {
    return ::std::all_of(
        m_side_coefficients.begin(), m_side_coefficients.end(),
        [&](::std::tuple<int, int, int> coefficients) {
            auto [a, b, c] = coefficients;
            return dot.x() * a + dot.y() * b + c <= 0;
        }
    );
}

// hexagon constructor from given center and radius
HexagonShape::HexagonShape(const Point &center, int radius) {
    // rounded multiplication by cos(pi/6)
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

void CircleShape::render(
    SDL_Renderer *renderer,
    SDL_Color fill_color,
    int x_offset,
    int y_offset
) const {
    filledCircleRGBA(
        renderer, m_center.x() + x_offset, m_center.y() + y_offset, m_radius,
        fill_color.r, fill_color.g, fill_color.b, fill_color.a
    );
}

void CircleShape::render_border(
    SDL_Renderer *renderer,
    SDL_Color border_color,
    int x_offset,
    int y_offset
) const {
    circleRGBA(
        renderer, m_center.x() + x_offset, m_center.y() + y_offset, m_radius,
        border_color.r, border_color.g, border_color.b, border_color.a
    );
}

bool CircleShape::in_bounds(const Point &dot) const {
    int dx = (m_center.x() - dot.x());
    int dy = (m_center.y() - dot.y());
    return dx * dx + dy * dy < m_radius * m_radius;
}

RectangleShape::RectangleShape(int x, int y, int width, int height) {
    m_vertexes.emplace_back(x, y);
    m_vertexes.emplace_back(x + width, y);
    m_vertexes.emplace_back(x + width, y + height);
    m_vertexes.emplace_back(x, y + height);
    init_side_coefficients();
}
}  // namespace runebound::graphics