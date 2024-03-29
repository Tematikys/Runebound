#include <graphics_client.hpp>

namespace runebound::graphics {
void Client::init_character_list_window() {
    auto window = std::make_unique<Window>(
        Window(WINDOW_WIDTH, WINDOW_HEIGHT, {0xFF, 0xFF, 0xFF, 0xFF})
    );
    m_window.add_window("char_list", std::move(window), {0, 0}, false, false);
}

void Client::update_character_list_window() {
    auto *win = m_window.get_window("char_list");

    win->remove_all_buttons();

    {  //  MAIN MENU BUTTON
        Texture texture;
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"], "Main menu",
            {0x00, 0x00, 0x00, 0xFF}
        );
        Button button(
            10 * 18, 30, HorizontalButtonTextureAlign::CENTER,
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
            "main_menu", button,
            {WINDOW_WIDTH - 10 * 18 - 5, WINDOW_HEIGHT - 35 * 2}, true, true
        );
    }  //  MAIN MENU BUTTON

    {  //  EXIT BUTTON
        Texture texture;
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"], "Exit",
            {0x00, 0x00, 0x00, 0xFF}
        );
        Button button(
            10 * 18, 30, HorizontalButtonTextureAlign::CENTER,
            VerticalButtonTextureAlign::CENTER, 0, 0, texture,
            [this]() {
                m_network_client.exit_game();
                m_is_running = false;
            },
            []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
        );
        win->add_button(
            "exit", button,
            {WINDOW_WIDTH - 10 * 18 - 5, WINDOW_HEIGHT - 30 - 5}, true, true
        );
    }  //  EXIT BUTTON

    int count = 0;
    for (const auto &character :
         m_network_client.get_game_client().m_remaining_standard_characters) {
        const auto &name = STANDARD_CHARACTER_TO_STRING.at(character);
        Texture texture;
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"], name,
            {0x00, 0x00, 0x00, 0xFF}
        );
        Button button(
            texture.width(), 30, HorizontalButtonTextureAlign::CENTER,
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
        win->add_button(name, button, {5, 35 + count * 35 + 5}, true, true);
        ++count;
    }

    Texture texture1;
    texture1.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Create new character",
        {0x00, 0x00, 0x00, 0xFF}
    );
    win->add_texture("Create new character", texture1, {5, 5}, true);

    if (!m_network_client.get_game_client().m_free_characters.empty()) {
        count = 0;
        for (const auto &character :
             m_network_client.get_game_client().m_free_characters) {
            const auto &name = STANDARD_CHARACTER_TO_STRING.at(character);
            Texture texture;
            texture.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono30"], name,
                {0x00, 0x00, 0x00, 0xFF}
            );
            Button button(
                texture.width(), 30, HorizontalButtonTextureAlign::CENTER,
                VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                [character, this]() {
                    m_network_client.select_free_character(character);
                    m_window.set_updatability_window("char_list", false);
                    m_window.set_visibility_window("char_list", false);
                    m_window.set_active_window("game");
                    m_window.set_updatability_window("game", true);
                    m_window.set_visibility_window("game", true);
                },
                []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
            );
            win->add_button(
                name, button, {605, 35 + count * 35 + 5}, true, true
            );
            ++count;
        }

        Texture texture2;
        texture2.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"],
            "Select existing character", {0x00, 0x00, 0x00, 0xFF}
        );
        win->add_texture("Select existing character", texture2, {605, 5}, true);
    }
}
}  // namespace runebound::graphics