#include <graphics_client.hpp>

namespace runebound::graphics {
void Client::init_main_menu_window() {
    {  // MAIN MENU WINDOW
        auto *win = &m_window;
        auto window = ::std::make_unique<Window>(Window(
            m_graphic_renderer, WINDOW_WIDTH, WINDOW_HEIGHT,
            {0xFF, 0xFF, 0xFF, 0xFF}
        ));
        Texture texture;
        Button button;
        TextField text_field;

        {  // ===== TEXT FIELD BUTTON =====
            button = Button(
                32 * 18, 30, HorizontalButtonTextureAlign::CENTER,
                VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                [this]() {
                    m_window.get_window("main_menu")
                        ->set_active_text_field("new_game");
                },
                []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
            );
            text_field = TextField("New game", button, 32);
            window->add_text_field(
                "new_game", text_field, m_fonts["FreeMono30"],
                {0x00, 0x00, 0x00, 0xFF}, {5, 5}, true, true
            );
        }  // ===== TEXT FIELD BUTTON =====

        {  // ===== ADD GAME BUTTON =====
            texture.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono30"], "Add game",
                {0x00, 0x00, 0x00, 0xFF}
            );
            button = Button(
                8 * 18, 30, HorizontalButtonTextureAlign::CENTER,
                VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                [this]() {
                    if (!m_window.get_window("main_menu")
                             ->get_text_field("new_game")
                             ->get()
                             .empty()) {
                        m_network_client.add_game(
                            m_window.get_window("main_menu")
                                ->get_text_field("new_game")
                                ->get()
                        );
                        m_window.get_window("main_menu")
                            ->get_text_field("new_game")
                            ->clear();
                    }
                },
                []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
            );
            window->add_button("add_game", button, {586, 5}, true, true);
        }  // ===== ADD GAME BUTTON =====

                {  // ===== EXIT BUTTON =====
                    texture.load_text_from_string(
                        m_graphic_renderer, m_fonts["FreeMono30"], "Exit",
                        {0x00, 0x00, 0x00, 0xFF}
                    );
                    button = Button(
                        8 * 18, 30, HorizontalButtonTextureAlign::CENTER,
                        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                        [this]() { m_is_running = false; }, []() {},
                        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
                    );
                    window->add_button("exit", button, {586, 40}, true, true);
                }  // ===== EXIT BUTTON =====
        window->activate();

        win->add_window("main_menu", ::std::move(window), {0, 0}, true, true);
    }  // MAIN MENU WINDOW
    {  // GAME LIST WINDOW
        auto *win = &m_window;
        auto window = ::std::make_unique<Window>(
            Window(m_graphic_renderer, 32 * 18, 355, {255, 255, 255, 255})
        );
        win->get_window("main_menu")
            ->add_window("game_list", ::std::move(window), {5, 40}, true, true);
    }  // GAME LIST WINDOW
}

void Client::update_main_menu_window() {
        auto *win = m_window.get_window("main_menu")->get_window("game_list");
    win->remove_all_buttons();
    for (::std::size_t i = m_game_list_start_index;
         i < ::std::min(
                 m_game_list_start_index + m_game_list_show_amount,
                 m_network_client.get_game_names_size()
             );
         ++i) {
        Texture texture;
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"],
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
            {5, static_cast<int>(i - m_game_list_start_index) * 35 + 5}, true,
            true
        );
    }
}
}  // namespace runebound::graphics
