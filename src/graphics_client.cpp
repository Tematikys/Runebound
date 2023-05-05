// #define DEBUG_INFO
#include <graphics_client.hpp>
#include <iostream>
#include <memory>
#include <utility>

namespace runebound::graphics {
void Client::load_fonts() {
    for (auto [path, font_name] : FONTS) {
        for (int i = 1; i < 201; ++i) {
            const ::std::string name = font_name + ::std::to_string(i);
            m_fonts[name] = nullptr;
            load_font(m_fonts[name], path, i);
            if (m_fonts[name] == nullptr) {
                ::std::cout << "Failed to load: " << name << ::std::endl;
                return;
            }
        }
    }
}

void Client::load_images() {
    for (auto [path, name] : IMAGES) {
        m_images[name] = Texture();
        m_images[name].load_image_from_file(m_graphic_renderer, path);
    }
}

void Client::init() {
    init_graphics();
    init_main_menu();
    //    init_game();
    m_window.set_active_window("main_menu");
    m_window.activate();
}

void Client::init_graphics() {
    if (!SDL_init(m_graphic_window, m_graphic_renderer)) {
        ::std::cout << "Failed to inti SDL!" << ::std::endl;
        return;
    }
    m_is_running = true;
    m_frame_time = 1000 / WINDOW_FPS;
    m_main_menu_active_text_field = 0;
    load_fonts();
    load_images();
}

void Client::init_board() {
    m_board = Board(m_network_client.get_game_client().m_map);
}

void Client::init_game() {
    Texture texture;
    // ===== MAIN MENU BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Main menu",
        {0x00, 0x00, 0x00, 0xFF}
    );
    m_game_button_pos.emplace_back(615, 5);
    m_game_buttons.push_back(Button(
        180, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() {
            m_network_client.exit_game();
            m_joined_to_game = false;
            m_character_selected = false;
        },
        []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    ));
    // ===== MAIN MENU BUTTON =====
    // ===== EXIT BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Exit",
        {0x00, 0x00, 0x00, 0xFF}
    );
    m_game_button_pos.emplace_back(615, 40);
    m_game_buttons.push_back(Button(
        180, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() {
            m_is_running = false;
            m_network_client.exit_game();
        },
        []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    ));
    // ===== EXIT BUTTON =====
    // ===== THROW DICE BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Throw dice",
        {0x00, 0x00, 0x00, 0xFF}
    );
    m_game_button_pos.emplace_back(615, 765);
    m_game_buttons.push_back(Button(
        180, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() { m_network_client.throw_move_dice(); }, []() {},
        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    ));
    // ===== THROW DICE BUTTON =====
    // ===== RELAX BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Relax",
        {0x00, 0x00, 0x00, 0xFF}
    );
    m_game_button_pos.emplace_back(615, 730);
    m_game_buttons.push_back(Button(
        180, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() { m_network_client.relax(); }, []() {},
        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    ));
    // ===== RELAX BUTTON =====
    // ===== PASS BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Pass",
        {0x00, 0x00, 0x00, 0xFF}
    );
    m_game_button_pos.emplace_back(615, 695);
    m_game_buttons.push_back(Button(
        180, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() { m_network_client.pass(); }, []() {},
        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    ));
    // ===== PASS BUTTON =====
}

void Client::init_main_menu() {
    auto window = ::std::make_unique<Window>(Window(
        m_graphic_renderer, WINDOW_WIDTH, WINDOW_HEIGHT, {255, 255, 255, 255}
    ));
    Texture texture;
    Button button;
    TextField text_field;

    // ===== TEXT FIELD BUTTON =====
    button = Button(
        30 * 16, 50, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() {
            m_window.get_window("main_menu")->set_active_text_field("new_game");
        },
        []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    text_field = TextField("new game", button, 16);
    window->add_text_field(
        "new_game", text_field, m_fonts["FreeMono50"], {0, 0, 0, 255}, {35, 35}
    );
    // ===== TEXT FIELD BUTTON =====

    // ===== ADD GAME BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono50"], "Add game",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        8 * 30, 50, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() {
            if (!m_window.get_window("main_menu")
                     ->get_text_field("new_game")
                     ->get()
                     .empty()) {
                m_network_client.add_game(m_window.get_window("main_menu")
                                              ->get_text_field("new_game")
                                              ->get());
                m_window.get_window("main_menu")
                    ->get_text_field("new_game")
                    ->clear();
            }
        },
        []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button("add_game", button, {525, 35});
    // ===== ADD GAME BUTTON =====

    // ===== EXIT BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono50"], "Exit",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        4 * 30, 50, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() { m_is_running = false; }, []() {}, {0xFF, 0xFF, 0xFF, 0xFF},
        {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button("exit", button, {645, 715});
    // ===== EXIT BUTTON =====
    window->activate();

    m_window.add_window("main_menu", ::std::move(window), {0, 0});

    window = ::std::make_unique<Window>(
        Window(m_graphic_renderer, 730, 555, {255, 255, 255, 255})
    );
    m_window.get_window("main_menu")
        ->add_window("game_list", ::std::move(window), {35, 95});
}

void Client::game_handle_events(SDL_Event &event) {
#ifdef DEBUG_INFO
    ::std::cout << "[info] :: GAME HANDLE EVENTS" << ::std::endl;
#endif
    switch (event.type) {
        case SDL_QUIT:
            m_is_running = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            m_mouse_pressed = true;
            break;
    }
}

void Client::main_menu_handle_events(SDL_Event &event) {
#ifdef DEBUG_INFO
    ::std::cout << "[info] :: MAIN MENU HANDLE EVENTS" << ::std::endl;
#endif
    switch (event.type) {
        case SDL_QUIT:
            m_is_running = false;
            break;
        case SDL_MOUSEBUTTONDOWN:
            m_mouse_pressed = true;
            break;
        case SDL_TEXTINPUT:
            if (m_main_menu_active_text_field != 0 &&
                (((SDL_GetModState() & KMOD_CTRL) == 0) ||
                 (event.text.text[0] != 'c' && event.text.text[0] != 'C' &&
                  event.text.text[0] != 'v' && event.text.text[0] != 'V'))) {
                m_main_menu_text_fields[m_main_menu_active_text_field - 1].push(
                    event.text.text
                );
            }
            break;
        case SDL_KEYDOWN:
            if (m_main_menu_active_text_field == 0) {
                break;
            }
            if (event.key.keysym.sym == SDLK_BACKSPACE) {
                m_main_menu_text_fields[m_main_menu_active_text_field - 1].pop(
                );
            } else if (event.key.keysym.sym == SDLK_c && ((SDL_GetModState() & KMOD_CTRL) != 0)) {
                SDL_SetClipboardText(
                    m_main_menu_text_fields[m_main_menu_active_text_field - 1]
                        .get()
                        .c_str()
                );
            } else if (event.key.keysym.sym == SDLK_v && ((SDL_GetModState() & KMOD_CTRL) != 0)) {
                m_main_menu_text_fields[m_main_menu_active_text_field - 1]
                    .clear();
                m_main_menu_text_fields[m_main_menu_active_text_field - 1].push(
                    SDL_GetClipboardText()
                );
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

void Client::handle_events() {
#ifdef DEBUG_INFO
    ::std::cout << "[info] :: " << m_counter << " HANDLE EVENTS" << ::std::endl;
#endif
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                m_is_running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_mouse_pressed = true;
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
        m_window.handle_events(event);
    }
}

void Client::game_render() {
#ifdef DEBUG_INFO
    ::std::cout << "[info] :: GAME RENDER" << ::std::endl;
#endif
    if (m_character_selected) {
        m_board.render(m_graphic_renderer, 0, 0);
        for (const auto &character :
             m_network_client.get_game_client().m_characters) {
            const Point center = get_center_of_hexagon(
                character.get_position().x, character.get_position().y
            );
            ::std::string name = character.get_name();
            if (name == "Corbin") {
                name = "CORBIN";
            }
            if (name == "Elder Mok") {
                name = "ELDER_MOK";
            }
            if (name == "Laurel from Bloodwood") {
                name = "LAUREL_FROM_BLOODWOOD";
            }
            if (name == "Lissa") {
                name = "LISSA";
            }
            if (name == "Lord Hawthorne") {
                name = "LORD_HAWTHORNE";
            }
            if (name == "Master Thorn") {
                name = "MASTER_THORN";
            }
            auto &texture = m_images[name];
            texture.render(
                m_graphic_renderer, -texture.width() / 2 + center.x(),
                -texture.height() / 2 + center.y()
            );
        }
        int dx = 0;
        for (const auto &dice : m_network_client.get_last_dice_result()) {
            ::std::vector<Point> vertexes;
            PolygonShape tri;
            auto [key, col] = *DICE_COLOR.find(dice);
            const int size = 50;
            const int delay = 10;
            vertexes.emplace_back(
                (size + delay) + (size + delay) * dx,
                WINDOW_HEIGHT - (size + delay)
            );
            vertexes.emplace_back(
                delay + (size + delay) * dx, WINDOW_HEIGHT - delay
            );
            vertexes.emplace_back(
                delay + (size + delay) * dx, WINDOW_HEIGHT - (size + delay)
            );
            tri = PolygonShape{vertexes};
            tri.render(m_graphic_renderer, 0, 0, col.first);
            vertexes.pop_back();
            vertexes.emplace_back(
                delay + size + (size + delay) * dx, WINDOW_HEIGHT - delay
            );
            tri = PolygonShape{vertexes};
            tri.render(m_graphic_renderer, 0, 0, col.second);
            dx += 1;
        }
    } else {
        for (::std::size_t i = 0; i < m_character_list.size(); ++i) {
            m_character_list[i].render(
                m_graphic_renderer, m_character_list_pos[i].x(),
                m_character_list_pos[i].y()
            );
        }
    }
    for (::std::size_t i = 0; i < m_game_buttons.size(); ++i) {
        m_game_buttons[i].render(
            m_graphic_renderer, m_game_button_pos[i].x(),
            m_game_button_pos[i].y()
        );
    }
}

void Client::main_menu_render() {
#ifdef DEBUG_INFO
    ::std::cout << "[info] :: MAIN MENU RENDER" << ::std::endl;
#endif
    const RectangleShape rect(35, 95, 730, 565);
    rect.render_border(m_graphic_renderer, 0, 0, {0, 0, 0, 255});
    for (::std::size_t i = 0; i < m_game_list.size(); ++i) {
        m_game_list[i].render(
            m_graphic_renderer, m_game_list_pos[i].x(), m_game_list_pos[i].y()
        );
    }
    for (::std::size_t i = 0; i < m_main_menu_text_fields.size(); ++i) {
        m_main_menu_text_fields[i].render(
            m_graphic_renderer, m_fonts["FreeMono50"], {0, 0, 0, 255},
            m_main_menu_text_field_pos[i].x(), m_main_menu_text_field_pos[i].y()
        );
    }
    for (::std::size_t i = 0; i < m_main_menu_buttons.size(); ++i) {
        m_main_menu_buttons[i].render(
            m_graphic_renderer, m_main_menu_button_pos[i].x(),
            m_main_menu_button_pos[i].y()
        );
    }
}

void Client::render() {
#ifdef DEBUG_INFO
    ::std::cout << "[info] :: " << m_counter << " RENDER" << ::std::endl;
#endif
    SDL_SetRenderDrawColor(m_graphic_renderer, 255, 255, 255, 255);
    SDL_RenderClear(m_graphic_renderer);
    m_window.render(m_graphic_renderer, 0, 0);
    SDL_RenderPresent(m_graphic_renderer);
}

void Client::game_update() {
#ifdef DEBUG_INFO
    ::std::cout << "[info] :: GAME UPDATE" << ::std::endl;
#endif
    //    m_character_list.clear();
    //    m_character_list_pos.clear();
    //    if (!m_character_selected) {
    //        int i = 0;
    //        for (const auto &character : m_network_client.get_game_client()
    //                                         .m_remaining_standard_characters)
    //                                         {
    //            ::std::string name;
    //            switch (character) {
    //                case ::runebound::character::StandardCharacter::LISSA:
    //                    name = "LISSA";
    //                    break;
    //                case character::StandardCharacter::CORBIN:
    //                    name = "CORBIN";
    //                    break;
    //                case character::StandardCharacter::ELDER_MOK:
    //                    name = "ELDER MOK";
    //                    break;
    //                case character::StandardCharacter::LAUREL_FROM_BLOODWOOD:
    //                    name = "LAUREL FROM BLOODWOOD";
    //                    break;
    //                case character::StandardCharacter::LORD_HAWTHORNE:
    //                    name = "LORD HAWTHORNE";
    //                    break;
    //                case character::StandardCharacter::MASTER_THORN:
    //                    name = "MASTER THORN";
    //                    break;
    //            }
    //            Texture texture;
    //            texture.load_text_from_string(
    //                m_graphic_renderer, m_fonts["FreeMono30"], name,
    //                {0x00, 0x00, 0x00, 0xFF}
    //            );
    //            m_character_list_pos.emplace_back(0, i * (texture.height() +
    //            5)); Button button(
    //                texture.width(), texture.height(),
    //                HorizontalButtonTextureAlign::CENTER,
    //                VerticalButtonTextureAlign::CENTER, 0, 0, texture,
    //                [character, this]() {
    //                    ::std::cout << "CLicked" << ::std::endl;
    //                    m_character_selected = true;
    //                    m_network_client.select_character(character);
    //                },
    //                []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00,
    //                0xFF}
    //            );
    //            m_character_list.push_back(::std::move(button));
    //            ++i;
    //        }
    //    }
    //    for (::std::size_t i = 0; i < m_character_list.size(); ++i) {
    //        if (m_character_list[i].in_bounds(
    //                m_mouse_pos -
    //                Point(m_character_list_pos[i].x(),
    //                m_character_list_pos[i].y())
    //            )) {
    //            m_character_list[i].on_cover();
    //            if (m_mouse_pressed) {
    //                m_mouse_pressed = false;
    //                m_character_list[i].on_click();
    //            }
    //        }
    //    }
    //    for (::std::size_t i = 0; i < m_game_buttons.size(); ++i) {
    //        if (m_game_buttons[i].in_bounds(
    //                m_mouse_pos -
    //                Point(m_game_button_pos[i].x(), m_game_button_pos[i].y())
    //            )) {
    //            m_game_buttons[i].on_cover();
    //            if (m_mouse_pressed) {
    //                m_mouse_pressed = false;
    //                m_game_buttons[i].on_click();
    //            }
    //        }
    //    }
    //    if (m_character_selected) {
    //        m_board.update_selection(m_mouse_pos);
    //    }
    //    const ::std::size_t index = m_board.get_selected_hexagon();
    //    if (index != 0xFFFF && m_mouse_pressed) {
    //        ::std::cout << index << ::std::endl;
    //        m_network_client.make_move(
    //            static_cast<int>(index / ::runebound::map::STANDARD_SIZE),
    //            static_cast<int>(index % ::runebound::map::STANDARD_SIZE)
    //        );
    //        m_mouse_pressed = false;
    //    }
}

void Client::main_menu_update() {
#ifdef DEBUG_INFO
    ::std::cout << "[info] :: MAIN MENU UPDATE" << ::std::endl;
#endif
    //    m_game_list.clear();
    //    m_game_list_pos.clear();
    //    for (::std::size_t i = m_game_list_start_index;
    //         i < ::std::min(
    //                 m_game_list_start_index + m_game_list_show_amount,
    //                 m_network_client.get_game_names().size()
    //             );
    //         ++i) {
    //        Texture texture;
    //        texture.load_text_from_string(
    //            m_graphic_renderer, m_fonts["FreeMono50"],
    //            m_network_client.get_game_names()[i], {0x00, 0x00, 0x00, 0xFF}
    //        );
    //        m_game_list_pos.emplace_back(
    //            45, static_cast<int>(i - m_game_list_start_index) * 55 + 105
    //        );
    //        Button button(
    //            texture.width(), texture.height(),
    //            HorizontalButtonTextureAlign::CENTER,
    //            VerticalButtonTextureAlign::CENTER, 0, 0, texture,
    //            [i, this]() {
    //                m_network_client.join_game(m_network_client.get_game_names()[i]
    //                );
    //                m_joined_to_game = true;
    //                init_board();
    //            },
    //            []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    //        );
    //        m_game_list.push_back(::std::move(button));
    //    }
    //    for (::std::size_t i = 0; i < m_game_list.size(); ++i) {
    //        if (m_game_list[i].in_bounds(
    //                m_mouse_pos -
    //                Point(m_game_list_pos[i].x(), m_game_list_pos[i].y())
    //            )) {
    //            m_game_list[i].on_cover();
    //            if (m_mouse_pressed) {
    //                m_mouse_pressed = false;
    //                m_game_list[i].on_click();
    //            }
    //        }
    //    }
    //    for (::std::size_t i = 0; i < m_main_menu_text_fields.size(); ++i) {
    //        if (m_main_menu_text_fields[i].in_bounds(
    //                m_mouse_pos - Point(
    //                                  m_main_menu_text_field_pos[i].x(),
    //                                  m_main_menu_text_field_pos[i].y()
    //                              )
    //            )) {
    //            m_main_menu_text_fields[i].on_cover();
    //            if (m_mouse_pressed) {
    //                m_mouse_pressed = false;
    //                m_main_menu_text_fields[i].on_click();
    //            }
    //        }
    //    }
    //    if (m_mouse_pressed) {
    //        m_main_menu_active_text_field = 0;
    //    }
    //    for (::std::size_t i = 0; i < m_main_menu_buttons.size(); ++i) {
    //        if (m_main_menu_buttons[i].in_bounds(
    //                m_mouse_pos -
    //                Point(
    //                    m_main_menu_button_pos[i].x(),
    //                    m_main_menu_button_pos[i].y()
    //                )
    //            )) {
    //            m_main_menu_buttons[i].on_cover();
    //            if (m_mouse_pressed) {
    //                m_mouse_pressed = false;
    //                m_main_menu_buttons[i].on_click();
    //            }
    //        }
    //    }
}

void Client::update() {
#ifdef DEBUG_INFO
    ::std::cout << "[info] :: " << m_counter << " UPDATE" << ::std::endl;
#endif
    m_io_context.poll();
    update_mouse_pos(m_mouse_pos);
    auto active_window = m_window.get_active_window_name();
    if (active_window == "main_menu") {
        auto game_list_win =
            m_window.get_window("main_menu")->get_window("game_list");
        game_list_win->remove_all_buttons();
        for (::std::size_t i = m_game_list_start_index;
             i < ::std::min(
                     m_game_list_start_index + m_game_list_show_amount,
                     m_network_client.get_game_names().size()
                 );
             ++i) {
            Texture texture;
            texture.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono50"],
                m_network_client.get_game_names()[i], {0x00, 0x00, 0x00, 0xFF}
            );
            Button button(
                texture.width(), texture.height(),
                HorizontalButtonTextureAlign::CENTER,
                VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                [i, this]() {
                    m_network_client.join_game(m_network_client.get_game_names(
                    )[i]);
                    m_joined_to_game = true;
                    init_board();
                },
                []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
            );
            game_list_win->add_button(
                m_network_client.get_game_names()[i], button,
                {5, static_cast<int>(i - m_game_list_start_index) * 55 + 5}
            );
        }
    }
    m_window.update(m_mouse_pos, m_mouse_pressed);
    m_mouse_pressed = false;
    ++m_counter;
}

void Client::tick() {
#ifdef DEBUG_INFO
    ::std::cout << "[info] :: " << m_counter << " TICK" << ::std::endl;
#endif
    const uint32_t cur_frame_time = SDL_GetTicks();
    if (cur_frame_time < m_frame_time + m_prev_frame_time) {
        SDL_Delay(m_frame_time - cur_frame_time + m_prev_frame_time);
    }
    m_prev_frame_time = cur_frame_time;
}

void Client::exit() {
#ifdef DEBUG_INFO
    ::std::cout << "[info] :: " << m_counter << " EXIT" << ::std::endl;
#endif
    m_network_client.exit();
    for (auto &[name, font] : m_fonts) {
        TTF_CloseFont(font);
        font = nullptr;
    }
    for (auto &[name, image] : m_images) {
        image.free();
    }
    SDL_DestroyRenderer(m_graphic_renderer);
    m_graphic_renderer = nullptr;
    SDL_DestroyWindow(m_graphic_window);
    m_graphic_window = nullptr;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
}  // namespace runebound::graphics