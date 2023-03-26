#include <algorithm>
#include <graphics_segment.hpp>
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

// check that point is inside of polygon
bool PolygonShape::in_bounds(Point dot) const {
    return ::std::all_of(
        m_side_coefficients.begin(), m_side_coefficients.end(),
        [&](::std::tuple<int, int, int> coefficients) {
            auto [a, b, c] = coefficients;
            return dot.x() * a + dot.y() * b + c <= 0;
        }
    );
}

void PolygonShape::render(
    SDL_Renderer *renderer,
    SDL_Color fill_color,
    SDL_Color border_color
) const {
    SDL_SetRenderDrawColor(
        renderer, fill_color.r, fill_color.g, fill_color.b, fill_color.a
    );

    // get and set necessary variables
    ::std::vector<Point> vertexes = get_vertexes();
    const int num_vertexes = static_cast<int>(get_number_of_vertexes());
    int num_vertexes_processed = 1;

    // find the highest vertex
    int top_y = vertexes[0].y();
    int top_index = 0;
    for (int i = 1; i < num_vertexes; ++i) {
        if (vertexes[i].y() < top_y) {
            top_y = vertexes[i].y();
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
    left_x = right_x = (vertexes[top_index].x()) << 16;

    // left dx slope
    int left_slope = 0;
    if (vertexes[left_index].y() != vertexes[top_index].y()) {
        left_slope =
            ((vertexes[left_index].x() - vertexes[top_index].x()) << 16) /
            (vertexes[left_index].y() - vertexes[top_index].y());
    }
    // right dx slope
    int right_slope = 0;
    if (vertexes[right_index].y() != vertexes[top_index].y()) {
        right_slope =
            ((vertexes[right_index].x() - vertexes[top_index].x()) << 16) /
            (vertexes[right_index].y() - vertexes[top_index].y());
    }

    // y coordinate
    int y = vertexes[top_index].y();

    // until all the vertices are drawn, draw
    while (num_vertexes_processed < num_vertexes) {
        // while y is above side vertexes, draw horizontal lines
        while (y < vertexes[left_index].y() && y < vertexes[right_index].y()) {
            // draw line
            SDL_RenderDrawLine(renderer, left_x >> 16, y, right_x >> 16, y);
            // lower y
            ++y;
            // shift xs
            left_x += left_slope;
            right_x += right_slope;
        }

        // if y coordinate is on the same level as left vertex, go to next
        if (vertexes[left_index].y() <= y) {
            // make top vertex equal left
            top_index = left_index;
            // go to next vertex
            --left_index;
            // module
            if (left_index < 0) {
                left_index = num_vertexes - 1;
            }

            // change slope if needed
            if (vertexes[left_index].y() != vertexes[top_index].y()) {
                left_slope =
                    ((vertexes[left_index].x() - vertexes[top_index].x())
                     << 16) /
                    (vertexes[left_index].y() - vertexes[top_index].y());
            }

            // << 16 is used for rounding
            left_x = (vertexes[top_index].x()) << 16;
            // +1 processed vertex
            ++num_vertexes_processed;
        }

        // same for right side
        if (vertexes[right_index].y() <= y) {
            top_index = right_index;
            ++right_index;
            if (right_index == num_vertexes) {
                right_index = 0;
            }

            if (vertexes[right_index].y() != vertexes[top_index].y()) {
                right_slope =
                    ((vertexes[right_index].x() - vertexes[top_index].x())
                     << 16) /
                    (vertexes[right_index].y() - vertexes[top_index].y());
            }

            right_x = (vertexes[top_index].x()) << 16;
            ++num_vertexes_processed;
        }

        // draw connecting line
        SDL_RenderDrawLine(renderer, left_x >> 16, y, right_x >> 16, y);
    }

    SDL_SetRenderDrawColor(
        renderer, border_color.r, border_color.g, border_color.b, border_color.a
    );

    // draw every edge of polygon except last
    for (::std::size_t i = 0; i < get_number_of_vertexes(); ++i) {
        SDL_RenderDrawLine(
            renderer, get_vertex(i).x(), get_vertex(i).y(),
            get_vertex((i + 1) % get_number_of_vertexes()).x(),
            get_vertex((i + 1) % get_number_of_vertexes()).y()
        );
    }
}

// hexagon constructor from given center and radius
HexagonShape::HexagonShape(const Point &center, int radius) {
    // rounded multiplication by cos(pi/6)
    const int dx = (radius * 56756) >> 16;
    m_vertexes.emplace_back(center.x(), center.y() - radius);
    m_vertexes.emplace_back(center.x() + dx, center.y() - radius / 2);
    m_vertexes.emplace_back(center.x() + dx, center.y() + radius / 2);
    m_vertexes.emplace_back(center.x(), center.y() + radius);
    m_vertexes.emplace_back(center.x() - dx, center.y() + radius / 2);
    m_vertexes.emplace_back(center.x() - dx, center.y() - radius / 2);
    init_side_coefficients();
}
}  // namespace runebound::graphics