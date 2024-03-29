#include <graphics_client.hpp>
#include <iostream>
#include <string>

namespace runebound::graphics {
void Client::update_board() {
    m_board = Board(m_network_client.get_game_client().m_map);

    const auto &game = m_network_client.get_game_client();
    if (m_network_client.get_yourself_character() != nullptr &&
        &(game.m_characters[game.m_turn]) ==
            m_network_client.get_yourself_character()) {
        std::vector<Point> hexagons;
        for (const auto &e :
             m_network_client.get_game_client().m_possible_moves) {
            hexagons.emplace_back(e.x, e.y);
        }
        m_board.update_available_hexagons(hexagons);
    }
}

void Client::init_graphics() {
    if (!SDL_init(m_graphic_window, m_graphic_renderer)) {
        if (SHOW_CLIENT_DEBUG_INFO) {
            std::cout << "Failed to inti SDL!" << std::endl;
        }
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
            const std::string name = font_name + std::to_string(i);
            m_fonts[name] = nullptr;
            load_font(m_fonts[name], path, i);
            if (m_fonts[name] == nullptr) {
                if (SHOW_CLIENT_DEBUG_INFO) {
                    std::cout << "Failed to load: " << name << std::endl;
                }
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
    load_settings();
    init_graphics();
    m_window = Window(WINDOW_WIDTH, WINDOW_HEIGHT, {0xFF, 0xFF, 0xFF, 0xFF});
    if (SHOW_CLIENT_DEBUG_INFO) {
        std::cout << "===== Start init main menu =====" << std::endl;
    }
    init_main_menu_window();
    if (SHOW_CLIENT_DEBUG_INFO) {
        std::cout << "===== End init main menu =====" << std::endl;
    }
    init_character_list_window();
    init_game_window();
    init_fight_window();
    init_shop_window();
    init_inventory_window();
    m_window.set_active_window("main_menu");
    m_window.activate();
}

void Client::handle_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        switch (event.type) {
            case SDL_QUIT:
                m_is_running = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_need_to_update = true;
                m_mouse_pressed = true;
                break;
            case SDL_MOUSEWHEEL:
                m_need_to_update = true;
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
        if (m_window.handle_events(event)) {
            m_need_to_update = true;
        }
    }
}

void Client::render() {
    if (m_need_to_update) {
        SDL_SetRenderTarget(m_graphic_renderer, nullptr);
        SDL_SetRenderDrawBlendMode(m_graphic_renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(m_graphic_renderer, 255, 255, 255, 255);
        SDL_RenderClear(m_graphic_renderer);
        m_window.render(m_graphic_renderer, 0, 0);
        SDL_RenderPresent(m_graphic_renderer);
        m_need_to_update = false;
    }
}

void Client::update() {
    m_io_context.poll();
    m_prev_mouse_pos = m_mouse_pos;
    update_mouse_pos(m_mouse_pos);

    m_need_to_update |= m_network_client.is_game_need_update();

    if (m_prev_mouse_pos != m_mouse_pos) {
        m_need_to_update = true;
    }
    auto active_window = m_window.get_active_window_name();
    if (active_window == "main_menu") {
        update_main_menu_window();
    } else if (active_window == "char_list") {
        update_character_list_window();
    } else if (active_window == "game") {
        update_game_window();
    }
    if (m_window.update(m_mouse_pos, m_mouse_pressed)) {
        m_need_to_update = true;
    }
    m_mouse_pressed = false;
    ++m_counter;
    m_network_client.game_need_update = false;
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