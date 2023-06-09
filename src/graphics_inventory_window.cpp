#include <graphics_client.hpp>

namespace runebound::graphics {
void Client::init_inventory_window() {
    auto *win = m_window.get_window("game");
    auto window = std::make_unique<Window>(Window(
        WINDOW_WIDTH * 3 / 4, WINDOW_HEIGHT * 3 / 4, {0xFF, 0xFF, 0xFF, 0xFF}
    ));
    Texture texture;
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Exit",
        {0x00, 0x00, 0x00, 0xFF}
    );
    Button button(
        200, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() {
            m_window.get_window("game")->reset_active_window();
            m_window.get_window("game")->get_window("inventory")->deactivate();
            m_window.get_window("game")->set_visibility_window(
                "inventory", false
            );
            m_window.get_window("game")->set_updatability_window(
                "inventory", false
            );
            m_window.get_window("game")->set_all_updatability_button(true);
        },
        []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button(
        "exit", button, {window->width() - 205, window->height() - 35 * 1},
        true, true
    );
    win->add_window(
        "inventory", std::move(window), {WINDOW_WIDTH / 8, WINDOW_HEIGHT / 8},
        false, false
    );
}

void Client::update_inventory_window() {
    if (m_network_client.get_yourself_character() == nullptr) {
        return;
    }
    auto *win = m_window.get_window("game")->get_window("inventory");
    win->remove_all_textures();
}
}  // namespace runebound::graphics