#include "graphics_client.hpp"
#include <iostream>

namespace runebound::client {
void Client::init() {
    init_graphics();
}

void Client::init_graphics() {
    if (!::runebound::graphics::SDL_init(m_window, m_renderer)) {
        ::std::cout << "Failed to initialize!\n";
        return;
    }
    m_is_running = true;
    m_frame_time = 1000 / ::runebound::graphics::WINDOWS_FPS;

    load_fonts();

    //    m_text_fields.emplace_back();
    m_active_text_field = 0;

    ::runebound::graphics::Texture texture;
    texture.load_from_string(
        m_renderer, m_fonts["FreeMono50"], "                ",
        {0x00, 0x00, 0x00, 0xFF}
    );
    ::runebound::graphics::Button button(
        0, 0, texture.get_width(), texture.get_height(), 0, 0, texture,
        [&]() { m_active_text_field = 1; }, []() {}, {0xFF, 0xFF, 0xFF, 0xFF},
        {0x00, 0x00, 0x00, 0xFF}
    );
    ::runebound::graphics::TextField text_field("new game", button, 16);
    m_text_fields.push_back(::std::move(text_field));

    texture.load_from_string(
        m_renderer, m_fonts["FreeMono50"], "Add game", {0x00, 0x00, 0x00, 0xFF}
    );
    m_buttons.push_back(::runebound::graphics::Button(
        550, 0, texture.get_width(), texture.get_height(), 0, 0, texture,
        [&]() {
            m_network_client.add_game(m_text_fields[0].get());
            m_text_fields[0].clear();
        },
        []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    ));
    texture.load_from_string(
        m_renderer, m_fonts["FreeMono50"], "Exit", {0x00, 0x00, 0x00, 0xFF}
    );
    m_buttons.push_back(::runebound::graphics::Button(
        550, 50, texture.get_width(), texture.get_height(), 0, 0, texture,
        [&is_running = m_is_running]() { is_running = false; }, []() {},
        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    ));
}

void Client::init_board() {
    m_game = m_network_client.m_game_client;
    m_board = ::runebound::graphics::Board(m_game.m_map);
}

void Client::load_fonts() {
    for (auto [path, font_name] : ::runebound::graphics::FONTS) {
        for (int i = 1; i < 201; ++i) {
            ::std::string name = font_name + ::std::to_string(i);
            m_fonts[name] = nullptr;
            ::runebound::graphics::load_font(m_fonts[name], path, i);
            if (m_fonts[name] == nullptr) {
                ::std::cout << "Failed to load: " << name << ::std::endl;
            } else {
                ::std::cout << "Success to load: " << name << ::std::endl;
            }
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
            case SDL_MOUSEWHEEL:
                if (event.wheel.y < 0) {
                    if (m_start_game_index + m_show_amount <
                        m_network_client.get_game_names().size()) {
                        ++m_start_game_index;
                    }
                } else if (event.wheel.y > 0) {
                    if (m_start_game_index > 0) {
                        --m_start_game_index;
                    }
                }
        }
    }
}

void Client::render() {
    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(m_renderer);

    if (!m_joined_to_game) {
        for (const auto &button : m_games_to_render) {
            button.render(m_renderer);
        }

        for (const auto &field : m_text_fields) {
            field.render(
                m_renderer, m_fonts["FreeMono50"], {0, 0, 0, 255}, 0, 0
            );
        }

        for (const auto &button : m_buttons) {
            button.render(m_renderer);
        }
    } else {
        m_board.render(m_renderer);
    }
    SDL_RenderPresent(m_renderer);
}

void Client::update() {
    ::runebound::graphics::update_mouse_pos(m_mouse_pos);
    m_board.update_selection(::runebound::graphics::Point(m_mouse_pos));

    m_io_context.poll();

    if (!m_joined_to_game) {
        m_games_to_render.clear();

        for (::std::size_t i = m_start_game_index;
             i < ::std::min(
                     m_start_game_index + m_show_amount,
                     m_network_client.get_game_names().size()
                 );
             ++i) {
            ::runebound::graphics::Texture texture;
            ::runebound::graphics::generate_text(
                m_renderer, texture, m_network_client.get_game_names()[i],
                m_fonts["FreeMono50"], {0x00, 0x00, 0x00, 0xFF}
            );
            ::runebound::graphics::Button button(
                0,
                static_cast<int>(i - m_start_game_index) *
                        texture.get_height() +
                    100,
                texture.get_width(), texture.get_height(), 0, 0, texture,
                [i, this]() {
                    this->m_network_client.join_game(
                        this->m_network_client.get_game_names()[i],
                        ::runebound::character::StandardCharacter::LISSA
                    );
                    this->m_joined_to_game = true;
                    this->init_board();
                },
                []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
            );
            m_games_to_render.push_back(::std::move(button));
        }

        for (const auto &button : m_games_to_render) {
            if (button.in_bounds(::runebound::graphics::Point(m_mouse_pos))) {
                button.on_cover();
                if (m_mouse_pressed) {
                    m_mouse_pressed = false;
                    button.on_click();
                }
            }
        }

        for (const auto &field : m_text_fields) {
            if (field.in_bounds(::runebound::graphics::Point(m_mouse_pos))) {
                field.on_cover();
                if (m_mouse_pressed) {
                    m_mouse_pressed = false;
                    field.on_click();
                }
            }
        }

        if (m_mouse_pressed) {
            m_active_text_field = 0;
        }

        for (const auto &button : m_buttons) {
            if (button.in_bounds(::runebound::graphics::Point(m_mouse_pos))) {
                button.on_cover();
                if (m_mouse_pressed) {
                    m_mouse_pressed = false;
                    button.on_click();
                }
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