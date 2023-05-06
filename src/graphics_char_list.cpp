#include <graphics_client.hpp>

namespace runebound::graphics {
void Client::init_char_list() {
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
}  // namespace runebound::graphics