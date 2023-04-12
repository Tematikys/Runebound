#include "client.hpp"
#include <iostream>

namespace runebound::client {
void Client::init() {
    init_graphics();
    m_board = ::runebound::graphics::Board(m_map);
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
        m_renderer, m_fonts["OpenSans"], "Exit", {0xFF, 0x00, 0x00, 0xFF}
    );
    m_rect_buttons.push_back(::runebound::graphics::RectButton(
        700, 10, texture.get_width() + 10, texture.get_height() + 10, 5, 5,
        texture, [&]() { m_is_running = false; }, []() {},
        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    ));
    //
    //    tex.load_from_string(
    //        m_renderer, m_font, "ctrhtn", {0xFF, 0x00, 0x00, 0xFF}
    //    );
    //    m_buttons.push_back(::runebound::graphics::RectButton(
    //        700, 50, tex, [&]() { ::std::cout << "ghjdthrf" << m_counter <<
    //        '\n'; }, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    //    ));
    //
    //    m_network_client.add_game("test");
    //
    //    tex.load_from_string(m_renderer, m_font, "start", {0xFF, 0x00, 0x00,
    //    0xFF});
    //    ::runebound::graphics::RectButton rect(
    //        10, 10, tex,
    //        [&]() {
    //            ::std::cout << "work\n";
    //            m_active_window = ACTIVE_WINDOW::BOARD;
    //        },
    //        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    //    );
    //    m_main_menu.add_button(rect);
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
    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(m_renderer);

    /*int y_offset = 100;
    int x_offset = 100;
    for (const auto &texture : m_temp_textures_to_render) {
        texture.render(m_renderer, x_offset, y_offset);
        y_offset += texture.get_height();
    }*/

    //    switch (m_active_window) {
    //        case ACTIVE_WINDOW::BOARD:
    m_board.render(m_renderer);
    //    for (const auto &button : m_buttons) {
    //        button.render(m_renderer);
    //    }
    //            break;
    //        case ACTIVE_WINDOW::MAIN_MENU:
    //            m_main_menu.render(m_renderer);
    //            break;
    //    }

    for (const auto &button : m_rect_buttons) {
        button.render(m_renderer);
    }

    SDL_RenderPresent(m_renderer);
}

void Client::update() {
    ::runebound::graphics::update_mouse_pos(m_mouse_pos);
    m_board.update_selection(::runebound::graphics::Point(m_mouse_pos));
    //    m_temp_textures_to_render.clear();
    //
    //    m_io_context.poll();
    //
    //    for (const auto &game_name : m_network_client.game_names) {
    //        m_temp_textures_to_render.emplace_back();
    //        m_temp_textures_to_render.back().load_from_string(
    //            m_renderer, m_font, game_name, {0xFF, 0x00, 0x00, 0xFF}
    //        );
    //    }

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