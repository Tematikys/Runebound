#include "client.hpp"
#include <iostream>

namespace runebound::client {
void Client::init() {
    init_graphics();
    m_board = ::runebound::graphics::Board(m_game.m_map);
}

void Client::init_graphics() {
    if (!::runebound::graphics::SDL_init(m_window, m_renderer)) {
        ::std::cout << "Failed to initialize!\n";
        return;
    }
    m_is_running = true;
    m_frame_time = 1000 / ::runebound::graphics::WINDOWS_FPS;

    for (auto [path, font_name, size] : ::runebound::graphics::FONTS) {
        m_fonts[font_name] = nullptr;
        ::runebound::graphics::load_font(m_fonts[font_name], path, size);
    }
    ::runebound::graphics::Texture texture;
    texture.load_from_string(
        m_renderer, m_fonts["OpenSans"], "    ", {0x00, 0x00, 0x00, 0xFF}
    );
    ::runebound::graphics::RectButton button(
        10, 10, 10 + texture.get_width(), 10 + texture.get_height(), 5, 5,
        texture, []() { ::std::cout << "Click!\n"; }, []() {},
        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    m_text = ::std::move(::runebound::graphics::TextButton(
        button, m_fonts["OpenSans"], {0x00, 0x00, 0x00, 0xFF}
    ));
}

void Client::handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
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
    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(m_renderer);

    //    m_board.render(m_renderer);

    m_text.render(m_renderer);

    for (const auto &button : m_rect_buttons) {
        button.render(m_renderer);
    }

    SDL_RenderPresent(m_renderer);
}

void Client::update() {
    ::runebound::graphics::update_mouse_pos(m_mouse_pos);
    m_board.update_selection(::runebound::graphics::Point(m_mouse_pos));

    m_io_context.poll();
    if (m_mouse_pressed) {
        if (m_text.in_bounds(m_mouse_pos)) {
            m_text.on_click();
            m_mouse_pressed = false;
        } else {
            m_text.deactivate();
        }
    } else {
        if (m_text.in_bounds(m_mouse_pos)) {
            m_text.on_cover();
        }
    }

    for (const auto &button : m_rect_buttons) {
        if (button.in_bounds(::runebound::graphics::Point(m_mouse_pos))) {
            button.on_cover();
            if (m_mouse_pressed) {
                m_mouse_pressed = false;
                button.on_click();
            }
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

    for (auto &[name, font] : m_fonts) {
        TTF_CloseFont(font);
        font = nullptr;
    }

    for (auto &texture : m_textures) {
        texture.free();
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
}  // namespace runebound::client