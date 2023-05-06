// #define DEBUG_INFO
#include <graphics_client.hpp>
#include <iostream>
#include <memory>
#include <utility>

namespace runebound::graphics {
void Client::update_board() {
    m_board = Board(m_network_client.get_game_client().m_map);
}

void Client::init_graphics() {
    if (!SDL_init(m_graphic_window, m_graphic_renderer)) {
        ::std::cout << "Failed to inti SDL!" << ::std::endl;
        return;
    }
    m_is_running = true;
    m_frame_time = 1000 / WINDOW_FPS;
    load_fonts();
    load_images();
}

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
    init_game_list();
    init_game();
    m_window.set_active_window("main_menu");
    m_window.activate();
}

void Client::init_game() {
    auto window = ::std::make_unique<Window>(Window(
        m_graphic_renderer, WINDOW_WIDTH, WINDOW_HEIGHT,
        {0xFF, 0xFF, 0xFF, 0xFF}
    ));
    Texture texture;
    Button button;

    // ===== MAIN MENU BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Main menu",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        180, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() {
            m_network_client.exit_game();
            m_window.set_updatability_window("game", false);
            m_window.set_visibility_window("game", false);
            m_window.set_active_window("main_menu");
            m_window.set_updatability_window("main_menu", true);
            m_window.set_visibility_window("main_menu", true);
        },
        []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button("main_menu", button, {615, 5}, true, true);
    // ===== MAIN MENU BUTTON =====

    // ===== EXIT BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Exit",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        180, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() {
            m_network_client.exit_game();
            m_is_running = false;
        },
        []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button("exit", button, {615, 40}, true, true);
    // ===== EXIT BUTTON =====

    // ===== THROW DICE BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Throw dice",
        {0x00, 0x00, 0x00, 0xFF}
    );
    m_game_button_pos.emplace_back(615, 765);
    button = Button(
        180, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() { m_network_client.throw_move_dice(); }, []() {},
        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button("throw_dice", button, {615, 765}, true, true);
    // ===== THROW DICE BUTTON =====

    // ===== RELAX BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Relax",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        180, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() { m_network_client.relax(); }, []() {},
        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button("relax", button, {615, 730}, true, true);
    // ===== RELAX BUTTON =====

    // ===== PASS BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Pass",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        180, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() { m_network_client.pass(); }, []() {},
        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button("pass", button, {615, 695}, true, true);
    // ===== PASS BUTTON =====

    m_window.add_window("game", ::std::move(window), {0, 0}, false, false);
}

void Client::init_game_list() {
    auto window = ::std::make_unique<Window>(Window(
        m_graphic_renderer, WINDOW_WIDTH, WINDOW_HEIGHT,
        {0xFF, 0xFF, 0xFF, 0xFF}
    ));
    Texture texture;
    Button button;

    // ===== MAIN MENU BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Main menu",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        180, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() {
            m_network_client.exit_game();
            m_window.set_updatability_window("char_list", false);
            m_window.set_visibility_window("char_list", false);
            m_window.set_active_window("main_menu");
            m_window.set_updatability_window("main_menu", true);
            m_window.set_visibility_window("main_menu", true);
        },
        []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button(
        "main_menu", button, {WINDOW_WIDTH - 180 - 5, 5}, true, true
    );
    // ===== MAIN MENU BUTTON =====

    // ===== EXIT BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Exit",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        180, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() {
            m_network_client.exit_game();
            m_is_running = false;
        },
        []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button(
        "exit", button, {WINDOW_WIDTH - 180 - 5, 40}, true, true
    );
    // ===== EXIT BUTTON =====

    m_window.add_window("char_list", ::std::move(window), {0, 0}, false, false);
}

void Client::init_main_menu() {
    auto window = ::std::make_unique<Window>(Window(
        m_graphic_renderer, WINDOW_WIDTH, WINDOW_HEIGHT,
        {0xFF, 0xFF, 0xFF, 0xFF}
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
        "new_game", text_field, m_fonts["FreeMono50"], {0x00, 0x00, 0x00, 0xFF},
        {35, 35}, true, true
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
    window->add_button("add_game", button, {525, 35}, true, true);
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
    window->add_button("exit", button, {645, 715}, true, true);
    // ===== EXIT BUTTON =====
    window->activate();

    m_window.add_window("main_menu", ::std::move(window), {0, 0}, true, true);

    window = ::std::make_unique<Window>(
        Window(m_graphic_renderer, 730, 555, {255, 255, 255, 255})
    );
    m_window.get_window("main_menu")
        ->add_window("game_list", ::std::move(window), {35, 95}, true, true);
}

void Client::handle_events() {
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
    if (m_character_selected) {
        m_board.render(m_graphic_renderer, 0, 0);
    }
}

void Client::render() {
    SDL_SetRenderDrawColor(m_graphic_renderer, 255, 255, 255, 255);
    SDL_RenderClear(m_graphic_renderer);
    m_window.render(m_graphic_renderer, 0, 0);
    SDL_RenderPresent(m_graphic_renderer);
}

void Client::game_update() {
    update_board();
    m_board.update_selection(m_mouse_pos - m_board_pos);

    auto *win = m_window.get_window("game");

    SDL_Texture *tex = nullptr;
    m_board.render_to_texture(m_graphic_renderer, tex);
    for (const auto &character :
         m_network_client.get_game_client().m_characters) {
        const Point center = get_center_of_hexagon(
            character.get_position().x, character.get_position().y
        );
        const ::std::string name = character.get_name();
        m_images[name].render_to_texture(
            m_graphic_renderer, -m_images[name].width() / 2 + center.x(),
            -m_images[name].height() / 2 + center.y(), tex
        );
    }
    {
        Texture texture(tex);
        win->remove_texture("board");
        win->add_texture("board", texture, m_board_pos, true);
        SDL_DestroyTexture(tex);
        texture.free();
    }

    const int size = 50;
    const int delay = 10;
    const int amount =
        static_cast<int>(m_network_client.get_last_dice_result().size());
    tex = SDL_CreateTexture(
        m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        (size + delay) * amount - delay + 1, size + 1
    );
    SDL_SetRenderTarget(m_graphic_renderer, tex);
    SDL_SetRenderDrawColor(m_graphic_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(m_graphic_renderer);
    SDL_SetRenderTarget(m_graphic_renderer, nullptr);
    int dx = 0;
    for (const auto &dice : m_network_client.get_last_dice_result()) {
        ::std::vector<Point> vertexes;
        PolygonShape tri;
        auto [key, col] = *DICE_COLOR.find(dice);
        vertexes.emplace_back(size + (size + delay) * dx, 0);
        vertexes.emplace_back((size + delay) * dx, size);
        vertexes.emplace_back((size + delay) * dx, 0);
        tri = PolygonShape{vertexes};
        tri.render_to_texture(
            m_graphic_renderer, tex, col.first, {0x00, 0x00, 0x00, 0xFF}
        );
        vertexes.pop_back();
        vertexes.emplace_back(size + (size + delay) * dx, size);
        tri = PolygonShape{vertexes};
        tri.render_to_texture(
            m_graphic_renderer, tex, col.second, {0x00, 0x00, 0x00, 0xFF}
        );
        dx += 1;
    }
    {
        Texture texture(tex);
        win->remove_texture("dice");
        win->add_texture("dice", texture, {5, WINDOW_HEIGHT - size - 5}, true);
        SDL_DestroyTexture(tex);
        texture.free();
    }

    const ::std::size_t index = m_board.get_selected_hexagon();
    if (index != 0xFFFF && m_mouse_pressed) {
        ::std::cout << index << ::std::endl;
        m_network_client.make_move(
            static_cast<int>(index / ::runebound::map::STANDARD_SIZE),
            static_cast<int>(index % ::runebound::map::STANDARD_SIZE)
        );
        m_mouse_pressed = false;
    }
}

void Client::main_menu_update() {
    auto *win = m_window.get_window("main_menu")->get_window("game_list");
    win->remove_all_buttons();
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
                m_network_client.join_game(m_network_client.get_game_names()[i]
                );
                m_window.set_updatability_window("main_menu", false);
                m_window.set_visibility_window("main_menu", false);
                m_window.set_active_window("char_list");
                m_window.set_updatability_window("char_list", true);
                m_window.set_visibility_window("char_list", true);
            },
            []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
        );
        win->add_button(
            m_network_client.get_game_names()[i], button,
            {5, static_cast<int>(i - m_game_list_start_index) * 55 + 5}, true,
            true
        );
    }
}

void Client::char_list_update() {
    auto *win = m_window.get_window("char_list");
    win->remove_all_buttons();

    Texture texture;
    Button button;

    // ===== MAIN MENU BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Main menu",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        180, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() {
            m_network_client.exit_game();
            m_window.set_updatability_window("char_list", false);
            m_window.set_visibility_window("char_list", false);
            m_window.set_active_window("main_menu");
            m_window.set_updatability_window("main_menu", true);
            m_window.set_visibility_window("main_menu", true);
        },
        []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    win->add_button(
        "main_menu", button, {WINDOW_WIDTH - 180 - 5, 5}, true, true
    );
    // ===== MAIN MENU BUTTON =====

    // ===== EXIT BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Exit",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        180, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() {
            m_is_running = false;
            m_network_client.exit_game();
        },
        []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    win->add_button("exit", button, {WINDOW_WIDTH - 180 - 5, 40}, true, true);
    // ===== EXIT BUTTON =====

    int i = 0;
    for (const auto &character :
         m_network_client.get_game_client().m_remaining_standard_characters) {
        ::std::string name;
        switch (character) {
            case ::runebound::character::StandardCharacter::LISSA:
                name = "Lissa";
                break;
            case character::StandardCharacter::CORBIN:
                name = "Corbin";
                break;
            case character::StandardCharacter::ELDER_MOK:
                name = "Elder Mok";
                break;
            case character::StandardCharacter::LAUREL_FROM_BLOODWOOD:
                name = "Laurel from Bloodwood";
                break;
            case character::StandardCharacter::LORD_HAWTHORNE:
                name = "Lord Hawthorne";
                break;
            case character::StandardCharacter::MASTER_THORN:
                name = "Master Thorn";
                break;
        }
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"], name,
            {0x00, 0x00, 0x00, 0xFF}
        );
        button = Button(
            texture.width(), texture.height(),
            HorizontalButtonTextureAlign::CENTER,
            VerticalButtonTextureAlign::CENTER, 0, 0, texture,
            [character, this]() {
                m_network_client.select_character(character);
                m_window.set_updatability_window("char_list", false);
                m_window.set_visibility_window("char_list", false);
                m_window.set_active_window("game");
                m_window.set_updatability_window("game", true);
                m_window.set_visibility_window("game", true);
            },
            []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
        );
        win->add_button(
            name, button, {5, i * texture.height() + 5}, true, true
        );
        ++i;
    }
}

void Client::update() {
    m_io_context.poll();
    update_mouse_pos(m_mouse_pos);
    auto active_window = m_window.get_active_window_name();
    if (active_window == "main_menu") {
        main_menu_update();
    } else if (active_window == "char_list") {
        char_list_update();
    } else if (active_window == "game") {
        game_update();
    }
    m_window.update(m_mouse_pos, m_mouse_pressed);
    m_mouse_pressed = false;
    ++m_counter;
}

void Client::tick() {
    const uint32_t cur_frame_time = SDL_GetTicks();
    if (cur_frame_time < m_frame_time + m_prev_frame_time) {
        SDL_Delay(m_frame_time - cur_frame_time + m_prev_frame_time);
    }
    m_prev_frame_time = cur_frame_time;
}

void Client::exit() {
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