#include <graphics_client.hpp>
#include <set>

namespace runebound::graphics {
void Client::init_shop_window() {
    Texture texture;
    Button button;

    auto *win = m_window.get_window("game");
    auto window = ::std::make_unique<Window>(Window(
        m_graphic_renderer, win->width() * 3 / 4, win->height() * 3 / 4,
        {0xFF, 0xFF, 0xFF, 0xFF}
    ));

    {  // CLOSE BUTTON
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"], "Exit",
            {0x00, 0x00, 0x00, 0xFF}
        );
        button = Button(
            200, 30, HorizontalButtonTextureAlign::CENTER,
            VerticalButtonTextureAlign::CENTER, 0, 0, texture,
            [this]() {
                m_window.get_window("game")->get_window("shop")->deactivate();
                m_window.get_window("game")->set_visibility_window(
                    "shop", false
                );
                m_window.get_window("game")->set_updatability_window(
                    "shop", false
                );
            },
            []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
        );
        window->add_button(
            "exit", button, {window->width() - 205, window->height() - 35 * 1},
            true, true
        );
    }  //  CLOSE BUTTON

    win->add_window(
        "shop", ::std::move(window), {win->width() / 8, win->height() / 8},
        false, false
    );
}

void Client::update_shop_window() {
    const auto &game = m_network_client.get_game_client();
    if (m_network_client.m_character !=
        ::runebound::character::StandardCharacter::NONE) {
        const auto me = m_network_client.get_yourself_character();
        const auto &pos = me.get_position();
        const auto &cell =
            m_network_client.get_game_client().m_map.m_map[pos.x][pos.y];
        ::std::set<unsigned int> shop;
        if (m_network_client.get_game_client().m_shops.contains(pos)) {
            shop = m_network_client.get_game_client().m_shops.at(pos);
        }
    }
}
}  // namespace runebound::graphics