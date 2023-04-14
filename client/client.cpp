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

    load_fonts();

    m_text_fields.emplace_back();
    m_active_text_field = 0;

    ::runebound::graphics::Texture texture;
    texture.load_from_string(
        m_renderer, m_fonts["OpenSans50"], " ", {0x00, 0x00, 0x00, 0xFF}
    );
    m_buttons.push_back(::runebound::graphics::Button(
        10, 10, 10 + texture.get_width(), 10 + texture.get_height(), 5, 5,
        texture, [&]() { m_active_text_field = 1; }, []() {},
        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    ));
}

void Client::load_fonts() {
    for (auto [path, font_name] : ::runebound::graphics::FONTS) {
        for (int i = 1; i < 201; ++i) {
            ::std::string name = font_name + ::std::to_string(i);
            m_fonts[name] = nullptr;
            ::runebound::graphics::load_font(m_fonts[name], path, i);
        }
    }
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

            case SDL_TEXTINPUT:
                if (m_active_text_field != 0 &&
                    !(SDL_GetModState() & KMOD_CTRL &&
                      (event.text.text[0] == 'c' || event.text.text[0] == 'C' ||
                       event.text.text[0] == 'v' || event.text.text[0] == 'V')
                    )) {
                    m_text_fields[m_active_text_field - 1].push(event.text.text
                    );
                }
                break;
            case SDL_KEYDOWN:
                if (m_active_text_field == 0) {
                    break;
                }
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    m_text_fields[m_active_text_field - 1].pop();
                } else if (event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
                    SDL_SetClipboardText(
                        m_text_fields[m_active_text_field - 1].get().c_str()
                    );
                } else if (event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
                    m_text_fields[m_active_text_field - 1].clear();
                    m_text_fields[m_active_text_field - 1].push(
                        SDL_GetClipboardText()
                    );
                }
                break;
        }
    }
}

void Client::render() {
    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(m_renderer);

    //    m_board.render(m_renderer);

    for (const auto &field : m_text_fields) {
        field.render(
            m_renderer, m_fonts["OpenSans30"], {0, 0, 0, 255}, 100, 100
        );
    }

    for (const auto &button : m_buttons) {
        button.render(m_renderer);
    }

    SDL_RenderPresent(m_renderer);
}

void Client::update() {
    ::runebound::graphics::update_mouse_pos(m_mouse_pos);
    m_board.update_selection(::runebound::graphics::Point(m_mouse_pos));

    m_io_context.poll();

    for (const auto &button : m_buttons) {
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