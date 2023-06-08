#include <graphics_client.hpp>
#include <set>

namespace runebound::graphics {
void Client::init_shop_window() {
    Texture texture;
    Button button;

    auto *win = m_window.get_window("game");
    auto window = ::std::make_unique<Window>(
        Window(win->width(), win->height(), {0xFF, 0xFF, 0xFF, 0xFF})
    );

    {  // CLOSE BUTTON
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"], "Exit",
            {0x00, 0x00, 0x00, 0xFF}
        );
        button = Button(
            200, 30, HorizontalButtonTextureAlign::CENTER,
            VerticalButtonTextureAlign::CENTER, 0, 0, texture,
            [this]() {
                m_window.get_window("game")->reset_active_window();
                m_window.get_window("game")->get_window("shop")->deactivate();
                m_window.get_window("game")->set_visibility_window(
                    "shop", false
                );
                m_window.get_window("game")->set_updatability_window(
                    "shop", false
                );
                m_window.get_window("game")->set_all_updatability_button(true);
            },
            []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
        );
        window->add_button(
            "exit", button, {window->width() - 205, window->height() - 35 * 1},
            true, true
        );
    }  //  CLOSE BUTTON

    win->add_window("shop", ::std::move(window), {0, 0}, false, false);
}

void Client::update_shop_window() {
    if (m_network_client.get_yourself_character() == nullptr) {
        return;
    }
    auto *win = m_window.get_window("game")->get_window("shop");
    win->remove_all_textures();
    for (int i = 1; i <= 4; ++i) {
        win->remove_button(::std::to_string(i));
    }
    const auto &pos = m_network_client.get_yourself_character()->get_position();
    if (!m_network_client.get_game_client().m_shops.contains(pos)) {
        ::std::cout << "Trade does not exist" << ::std::endl;
        return;
    }

    static int selected_shop_item = 0;
    if (m_network_client.is_game_need_update()) {
        selected_shop_item = 0;
    }

    const ::std::set<unsigned int> shop =
        m_network_client.get_game_client().m_shops.at(pos);
    {  // UPDATE PRODUCTS
        int count = 0;
        for (auto e : shop) {
            auto prod = m_network_client.get_product(e);
            SDL_Texture *tex = SDL_CreateTexture(
                m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET, 300, 330
            );
            SDL_Color col;
            if (selected_shop_item == count + 1) {
                col = {0xBD, 0xCA, 0x03, 0xFF};
            } else {
                col = {0xFF, 0xFF, 0xFF, 0xFF};
            }
            SDL_SetRenderTarget(m_graphic_renderer, tex);
            SDL_SetRenderDrawColor(
                m_graphic_renderer, col.r, col.g, col.b, col.a
            );
            SDL_RenderClear(m_graphic_renderer);
            SDL_SetRenderTarget(m_graphic_renderer, nullptr);
            Texture texture;
            {  // BORDER
                const RectangleShape rect = RectangleShape(0, 0, 299, 329);
                rect.render_to_texture(
                    m_graphic_renderer, tex, col, {0x00, 0xFF, 0x00, 0xFF}
                );
                Texture temp;
                Button button(
                    300, 330, HorizontalButtonTextureAlign::CENTER,
                    VerticalButtonTextureAlign::CENTER, 0, 0, temp,
                    [&e = selected_shop_item, count, this]() {
                        if (e == count + 1) {
                            e = 0;
                        } else {
                            e = count + 1;
                        }
                        this->m_need_to_update = true;
                    },
                    []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
                );
                win->add_button(
                    std::to_string(count + 1), button, {5 + 305 * count, 5},
                    false, true
                );
            }  // BORDER
            const auto name = prod.get_product_name();
            {  // NAME
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    prod.get_product_name(), {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(
                    m_graphic_renderer, 101 - texture.width() / 2, 1, tex
                );
            }  // NAME
            {  // PRICE
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    "Price: " + ::std::to_string(prod.get_price()),
                    {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 1, 21, tex);
            }  // PRICE
            {  // MARKET PRICE
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    "Marker price: " +
                        ::std::to_string(prod.get_market_price()),
                    {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 1, 41, tex);
            }  // MARKET PRICE
            {  // DELTA HEATH
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    "Delta health: " +
                        ::std::to_string(prod.get_delta_max_health()),
                    {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 1, 61, tex);
            }  // DELTA HEALTH
            {  // DELTA SPEED
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    "Delta speed: " + ::std::to_string(prod.get_delta_speed()),
                    {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 1, 81, tex);
            }  // DELTA SPEED
            {  // DELTA HAND LIMIT
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    "Delta hand limit: " +
                        ::std::to_string(prod.get_delta_hand_limit()),
                    {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 1, 101, tex);
            }  // DELTA HAND LIMIT
            {  // CHARACTERISTICS
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    "Characteristics:", {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 1, 121, tex);
                auto chars = prod.get_delta_characteristic();
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    "    Body: " + ::std::to_string(
                                       chars[::runebound::Characteristic::BODY]
                                   ),
                    {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 1, 141, tex);
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    "    Intelligence: " +
                        ::std::to_string(
                            chars[::runebound::Characteristic::INTELLIGENCE]
                        ),
                    {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 1, 161, tex);
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    "    Spirit: " +
                        ::std::to_string(
                            chars[::runebound::Characteristic::INTELLIGENCE]
                        ),
                    {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 1, 181, tex);
            }  // CHARACTERISTICS
            {  // FIGHT TOKEN
                SDL_Texture *token_tex = SDL_CreateTexture(
                    m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_TARGET, 128, 128
                );
                SDL_SetRenderTarget(m_graphic_renderer, token_tex);
                SDL_SetRenderDrawColor(
                    m_graphic_renderer, col.r, col.g, col.b, col.a
                );
                SDL_RenderClear(m_graphic_renderer);
                SDL_SetRenderTarget(m_graphic_renderer, nullptr);
                const auto token = prod.get_fight_token();
                if (token) {
                    {  // BACKGROUND
                        m_images["fight_token"].render_to_texture(
                            m_graphic_renderer, 0, 0, token_tex
                        );
                    }  // BACKGROUND
                    {  // FACE SIDE
                        const bool init = token.value().first_lead;
                        const int num = token.value().first_count;
                        ::std::string token_name;
                        switch (token.value().first) {
                            case fight::HandFightTokens::PHYSICAL_DAMAGE:
                                token_name = "damage";
                                break;
                            case fight::HandFightTokens::MAGICAL_DAMAGE:
                                token_name = "magic";
                                break;
                            case fight::HandFightTokens::DEXTERITY:
                                token_name = "dexterity";
                                break;
                            case fight::HandFightTokens::HIT:
                                token_name = "hit";
                                break;
                            case fight::HandFightTokens::ENEMY_DAMAGE:
                                token_name = "skull";
                                break;
                            case fight::HandFightTokens::DOUBLING:
                                token_name = "double";
                                break;
                            case fight::HandFightTokens::SHIELD:
                                token_name = "shield";
                                break;
                            case fight::HandFightTokens::NOTHING:
                                token_name = "none";
                                break;
                        }
                        if (init) {
                            token_name += "_init";
                        }
                        m_images[token_name].render_to_texture(
                            m_graphic_renderer, 20, 20, token_tex
                        );
                        Texture token_texture;
                        token_texture.load_text_from_string(
                            m_graphic_renderer, m_fonts["FreeMono40"],
                            ::std::to_string(num), {0x00, 0x00, 0x00, 0xFF}
                        );
                        token_texture.render_to_texture(
                            m_graphic_renderer, 80, 30, token_tex
                        );
                    }  // FACE SIDE
                    {  // BACK SIDE
                        const bool init = token.value().second_lead;
                        const int num = token.value().second_count;
                        ::std::string token_name;
                        switch (token.value().second) {
                            case fight::HandFightTokens::PHYSICAL_DAMAGE:
                                token_name = "damage";
                                break;
                            case fight::HandFightTokens::MAGICAL_DAMAGE:
                                token_name = "magic";
                                break;
                            case fight::HandFightTokens::DEXTERITY:
                                token_name = "dexterity";
                                break;
                            case fight::HandFightTokens::HIT:
                                token_name = "hit";
                                break;
                            case fight::HandFightTokens::ENEMY_DAMAGE:
                                token_name = "skull";
                                break;
                            case fight::HandFightTokens::DOUBLING:
                                token_name = "double";
                                break;
                            case fight::HandFightTokens::SHIELD:
                                token_name = "shield";
                                break;
                            case fight::HandFightTokens::NOTHING:
                                token_name = "none";
                                break;
                        }
                        if (init) {
                            token_name += "_init";
                        }
                        m_images[token_name + "32"].render_to_texture(
                            m_graphic_renderer, 77, 77, token_tex
                        );
                        Texture token_texture;
                        token_texture.load_text_from_string(
                            m_graphic_renderer, m_fonts["FreeMono20"],
                            ::std::to_string(num), {0x00, 0x00, 0x00, 0xFF}
                        );
                        token_texture.render_to_texture(
                            m_graphic_renderer, 75, 100, token_tex
                        );
                    }  // BACK SIDE
                }
                texture = Texture(token_tex);
                texture.render_to_texture(m_graphic_renderer, 1, 201, tex);
            }  // FIGHT TOKEN
            texture = Texture(tex);
            win->add_texture(name, texture, {5 + 305 * count, 5}, true);
            SDL_DestroyTexture(tex);
            texture.free();
            ++count;
        }
    }  // UPDATE PRODUCTS
}
}  // namespace runebound::graphics