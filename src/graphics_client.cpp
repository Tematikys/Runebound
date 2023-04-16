#define DEBUG_INFO
#include "graphics_client.hpp"
#include <iostream>

namespace runebound::client {
void Client::init() {
    init_graphics();
    init_main_menu();
}

void Client::init_graphics() {
    if (!::runebound::graphics::SDL_init(m_window, m_renderer)) {
        ::std::cout << "Failed to inti SDL!" << ::std::endl;
        return;
    }
    m_is_running = true;
    m_frame_time = 1000 / ::runebound::graphics::WINDOWS_FPS;
    m_main_menu_active_text_field = 0;
    load_fonts();
}

void Client::init_main_menu() {
    ::runebound::graphics::Texture texture;

    // ===== TEXT FIELD BUTTON =====
    texture.load_from_string(
        m_renderer, m_fonts["FreeMono50"], "                ",
        {0x00, 0x00, 0x00, 0xFF}
    );
    ::runebound::graphics::Button button(
        35, 35, texture.get_width(), texture.get_height(), 0, 0, texture,
        [&]() { m_main_menu_active_text_field = 1; }, []() {},
        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    ::runebound::graphics::TextField text_field("new game", button, 16);
    m_main_menu_text_fields.push_back(::std::move(text_field));
    // ===== TEXT FIELD =====

    // ===== ADD GAME BUTTON =====
    texture.load_from_string(
        m_renderer, m_fonts["FreeMono50"], "Add game", {0x00, 0x00, 0x00, 0xFF}
    );
    m_main_menu_buttons.push_back(::runebound::graphics::Button(
        525, 35, texture.get_width(), texture.get_height(), 0, 0, texture,
        [&]() {
            m_network_client.add_game(m_main_menu_text_fields[0].get());
            m_main_menu_text_fields[0].clear();
        },
        []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    ));
    // ===== ADD GAME BUTTON =====

    // ===== EXIT BUTTON =====
    texture.load_from_string(
        m_renderer, m_fonts["FreeMono50"], "Exit", {0x00, 0x00, 0x00, 0xFF}
    );
    m_main_menu_buttons.push_back(::runebound::graphics::Button(
        645, 715, texture.get_width(), texture.get_height(), 0, 0, texture,
        [&is_running = m_is_running]() { is_running = false; }, []() {},
        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    ));
    // ===== EXIT BUTTON =====
}

void Client::init_board() {
    m_game = m_network_client.get_game_client();
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
                return;
            }
        }
    }
}

void Client::handle_events() {
#ifdef DEBUG_INFO
    ::std::cout << "[info] :: " << m_counter << " HANDLE EVENTS" << ::std::endl;
#endif
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
                if (m_main_menu_active_text_field != 0 &&
                    !(SDL_GetModState() & KMOD_CTRL &&
                      (event.text.text[0] == 'c' || event.text.text[0] == 'C' ||
                       event.text.text[0] == 'v' || event.text.text[0] == 'V')
                    )) {
                    m_main_menu_text_fields[m_main_menu_active_text_field - 1]
                        .push(event.text.text);
                }
                break;

            case SDL_KEYDOWN:
                if (m_main_menu_active_text_field == 0) {
                    break;
                }
                if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    m_main_menu_text_fields[m_main_menu_active_text_field - 1]
                        .pop();
                } else if (event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
                    SDL_SetClipboardText(m_main_menu_text_fields
                                             [m_main_menu_active_text_field - 1]
                                                 .get()
                                                 .c_str());
                } else if (event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
                    m_main_menu_text_fields[m_main_menu_active_text_field - 1]
                        .clear();
                    m_main_menu_text_fields[m_main_menu_active_text_field - 1]
                        .push(SDL_GetClipboardText());
                }
                break;

            case SDL_MOUSEWHEEL:
                if (event.wheel.y < 0) {
                    if (m_game_list_start_index + m_game_list_show_amount <
                        m_network_client.get_game_names().size()) {
                        ++m_game_list_start_index;
                    }
                } else if (event.wheel.y > 0) {
                    if (m_game_list_start_index > 0) {
                        --m_game_list_start_index;
                    }
                }
        }
    }
}

void Client::game_render() {
    m_board.render(m_renderer);
}

void Client::main_menu_render() {
    ::runebound::graphics::RectangleShape rect(35, 95, 730, 565);
    rect.render_border(m_renderer, {0, 0, 0, 255});
    for (const auto &button : m_game_list) {
        button.render(m_renderer);
    }
    for (const auto &field : m_main_menu_text_fields) {
        field.render(m_renderer, m_fonts["FreeMono50"], {0, 0, 0, 255});
    }
    for (const auto &button : m_main_menu_buttons) {
        button.render(m_renderer);
    }
}

void Client::render() {
#ifdef DEBUG_INFO
    ::std::cout << "[info] :: " << m_counter << " RENDER" << ::std::endl;
#endif
    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(m_renderer);

    if (m_is_joined_to_game) {
        game_render();
    } else {
        main_menu_render();
    }

    SDL_RenderPresent(m_renderer);
}

void Client::game_update() {
    return;
}

void Client::main_menu_update() {
    m_game_list.clear();

    for (::std::size_t i = m_game_list_start_index;
         i < ::std::min(
                 m_game_list_start_index + m_game_list_show_amount,
                 m_network_client.get_game_names().size()
             );
         ++i) {
        ::runebound::graphics::Texture texture;
        ::runebound::graphics::generate_text(
            m_renderer, texture, m_network_client.get_game_names()[i],
            m_fonts["FreeMono50"], {0x00, 0x00, 0x00, 0xFF}
        );
        ::runebound::graphics::Button button(
            45,
            static_cast<int>(i - m_game_list_start_index) *
                    (texture.get_height() + 5) +
                105,
            texture.get_width(), texture.get_height(), 0, 0, texture,
            [i, this]() {
                this->m_network_client.join_game(
                    this->m_network_client.get_game_names()[i]
                );
                this->m_is_joined_to_game = true;
                this->init_board();
            },
            []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
        );
        m_game_list.push_back(::std::move(button));
    }

    for (const auto &button : m_game_list) {
        if (button.in_bounds(::runebound::graphics::Point(m_mouse_pos))) {
            button.on_cover();
            if (m_mouse_pressed) {
                m_mouse_pressed = false;
                button.on_click();
            }
        }
    }

    for (const auto &field : m_main_menu_text_fields) {
        if (field.in_bounds(::runebound::graphics::Point(m_mouse_pos))) {
            field.on_cover();
            if (m_mouse_pressed) {
                m_mouse_pressed = false;
                field.on_click();
            }
        }
    }

    if (m_mouse_pressed) {
        m_main_menu_active_text_field = 0;
    }

    for (const auto &button : m_main_menu_buttons) {
        if (button.in_bounds(::runebound::graphics::Point(m_mouse_pos))) {
            button.on_cover();
            if (m_mouse_pressed) {
                m_mouse_pressed = false;
                button.on_click();
            }
        }
    }
}

void Client::update() {
#ifdef DEBUG_INFO
    ::std::cout << "[info] :: " << m_counter << " UPDATE" << ::std::endl;
#endif
    m_io_context.poll();

    ::runebound::graphics::update_mouse_pos(m_mouse_pos);
    m_board.update_selection(::runebound::graphics::Point(m_mouse_pos));

    if (m_is_joined_to_game) {
        game_update();
    } else {
        main_menu_update();
    }

    m_mouse_pressed = false;
    ++m_counter;
}

void Client::tick() {
#ifdef DEBUG_INFO
    ::std::cout << "[info] :: " << m_counter << " TICK" << ::std::endl;
#endif
    uint32_t cur_frame_time = SDL_GetTicks();
    if (cur_frame_time < m_frame_time + m_prev_frame_time) {
        SDL_Delay(m_frame_time - cur_frame_time + m_prev_frame_time);
    }
    m_prev_frame_time = cur_frame_time;
}

void Client::exit() {
#ifdef DEBUG_INFO
    ::std::cout << "[info] :: " << m_counter << " EXIT" << ::std::endl;
#endif
    m_game_list.clear();
    m_main_menu_text_fields.clear();
    m_main_menu_buttons.clear();
    for (auto &[name, font] : m_fonts) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    for (auto &texture : m_main_menu_textures) {
        texture.free();
    }

    SDL_DestroyRenderer(m_renderer);
    m_renderer = nullptr;
    SDL_DestroyWindow(m_window);
    m_window = nullptr;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
}  // namespace runebound::client