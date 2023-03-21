#include "client.hpp"
#include <iostream>

namespace runebound::client {
void Client::init_graphics(
    const char *title,
    int x_pos,
    int y_pos,
    int width,
    int height,
    int fps
) {
    if (!::runebound::graphics::SDL_init(
            m_window, m_renderer, title, x_pos, y_pos, width, height
        )) {
        ::std::cout << "Failed to initialize!\n";
        return;
    }
    m_is_running = true;
    m_fps = 1000 / fps;
}

void Client::init_board(const ::runebound::map::Map &map) {
    m_board = ::runebound::graphics::Board(map);
}

void Client::handle_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            m_is_running = false;
        }
    }
}

void Client::render() {
    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(m_renderer);

    int x{};
    int y{};
    SDL_GetMouseState(&x, &y);

    m_board.set_selected_hexagon(0xFFFF);
    if (auto index = m_board.in_bounds(::runebound::graphics::Point(x, y))) {
        m_board.set_selected_hexagon(index.value());
    }

    m_board.render(m_renderer);

    SDL_RenderPresent(m_renderer);
}

void Client::update() {
    ++m_counter;
}

void Client::tick() {
    uint32_t cur_frame_time = SDL_GetTicks();
    if (cur_frame_time - m_prev_frame_time < m_fps) {
        SDL_Delay(m_fps - cur_frame_time + m_prev_frame_time);
    }
    m_prev_frame_time = cur_frame_time;
}

void Client::exit() {
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();
}
}  // namespace runebound::client