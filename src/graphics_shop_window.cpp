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
    if (m_network_client.get_yourself_character() != nullptr) {
        const auto me = m_network_client.get_yourself_character();
        const auto &pos = me->get_position();
        const auto &cell =
            m_network_client.get_game_client().m_map.m_map[pos.x][pos.y];
        ::std::set<unsigned int> shop;
//        if (m_network_client.get_game_client().m_shops.contains(pos)) {
//            shop = m_network_client.get_game_client().m_shops.at(pos);
//            for (auto e : shop) {
//                {  // UPDATE PRODUCT
//                    auto prod = m_network_client.get_product(e);
//                    SDL_Texture *tex = nullptr;
//                    Texture res;
//                    Texture texture;
//                    RectangleShape rect;
//                    const auto name = prod.get_product_name();
//                    const auto price = prod.get_price();
//                    const auto market_price = prod.get_price();
//                    const auto health = prod.get_delta_max_health();
//                    {  // NAME
//                        texture.load_text_from_string(
//                            m_graphic_renderer, m_fonts["FreeMono20"], name,
//                            {0x00, 0x00, 0x00, 0xFF}
//                        );
//                        texture.render_to_texture();
//                    }  // NAME
//                    {  // GOLD NUM
//                        texture.load_text_from_string(
//                            m_graphic_renderer, m_fonts["FreeMono20"], gold,
//                            {0x00, 0x00, 0x00, 0xFF}
//                        );
//                        window->remove_texture(name + gold);
//                        window->add_texture(
//                            name + gold, texture, {25, 20}, true
//                        );
//                    }  // GOLD NUM
//                    {  // HEALTH NUM
//                        texture.load_text_from_string(
//                            m_graphic_renderer, m_fonts["FreeMono20"], health,
//                            {0x00, 0x00, 0x00, 0xFF}
//                        );
//                        window->remove_texture(name + health);
//                        window->add_texture(
//                            name + health, texture, {25, 40}, true
//                        );
//                    }  // HEALTH NUM
//                    {  // BORDER
//                        rect = RectangleShape(0, 0, 2 * 10 * 30 * 3 / 5, 60);
//                        tex = SDL_CreateTexture(
//                            m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
//                            SDL_TEXTUREACCESS_TARGET, 2 * 10 * 30 * 3 / 5 + 1,
//                            60 + 1
//                        );
//                        SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
//                        rect.render_to_texture(
//                            m_graphic_renderer, tex, {0xFF, 0xFF, 0xFF, 0xFF},
//                            {0x00, 0xFF, 0x00, 0xFF}
//                        );
//                        texture = Texture(tex);
//                        window->remove_texture("_" + name + "border");
//                        window->add_texture(
//                            " " + name + "border", texture, {0, 0}, true
//                        );
//                    }  // BORDER
//                    SDL_DestroyTexture(tex);
//                    texture.free();
//                }  // UPDATE PRODUCT
//            }
//        }
    }
}
}  // namespace runebound::graphics
