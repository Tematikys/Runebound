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
    m_frame_time = 1000 / fps;

    ::runebound::graphics::load_font(m_font, "../../data/fonts/lazy.ttf", 50);

    ::runebound::graphics::Texture tex;
    tex.load_from_string(
        m_renderer, m_font, "button", {0xFF, 0x00, 0x00, 0xFF}
    );
    m_buttons.push_back(::runebound::graphics::RectButton(
        100, 100, tex, []() { ::std::cout << "button clicked\n"; },
        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    ));
}

void Client::init_board(const ::runebound::map::MapClient &map) {
    m_board = ::runebound::graphics::Board(map);
}

void Client::handle_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                m_is_running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_mouse_pressed = true;
                break;
        }
    }
}

void Client::render() {
    SDL_SetRenderDrawColor(m_renderer, 0xEE, 0xEE, 0xEE, 0xFF);
    SDL_RenderClear(m_renderer);

    //    m_board.render(m_renderer);

    int y_offset = 100;
    int x_offset = 100;
    for (const auto &texture : m_temp_textures_to_render) {
        texture.render(m_renderer, x_offset, y_offset);
        y_offset += texture.get_height();
    }

    for (const auto &button : m_buttons) {
        button.render(m_renderer);
    }

    SDL_RenderPresent(m_renderer);
}

void Client::update() {
    ::runebound::graphics::update_mouse_pos(m_mouse_pos);
    m_board.update_selection(::runebound::graphics::Point(m_mouse_pos));
    m_temp_textures_to_render.clear();

    //    m_io_context.poll();

    for (const auto &game_name : m_network_client.game_names) {
        m_temp_textures_to_render.emplace_back();
        m_temp_textures_to_render.back().load_from_string(
            m_renderer, m_font, game_name, {0xFF, 0x00, 0x00, 0xFF}
        );
    }

    for (const auto &button : m_buttons) {
        if (button.in_bounds(::runebound::graphics::Point(m_mouse_pos)) &&
            m_mouse_pressed) {
            m_mouse_pressed = false;
            button.on_click();
        }
    }

    m_mouse_pressed = false;
    ++m_counter;
}

void Client::tick() {
    uint32_t cur_frame_time = SDL_GetTicks();
    if (cur_frame_time < m_frame_time + m_prev_frame_time) {
        SDL_Delay(m_frame_time - cur_frame_time + m_prev_frame_time);
    }
    m_prev_frame_time = cur_frame_time;
}

void Client::exit() {
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    m_window = nullptr;
    m_renderer = nullptr;

    SDL_Quit();
}
}  // namespace runebound::client