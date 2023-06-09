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

    auto prods =
        m_network_client.get_yourself_character()->get_products();
    {  // UPDATE PRODUCTS
        int count = 0;
        for (auto e : prods) {
            auto prod = m_network_client.get_product(e);
            SDL_Texture *tex = SDL_CreateTexture(
                m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET, 300, 330
            );
            SDL_Color col = {0xFF, 0xFF, 0xFF, 0xFF};
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
                    "Price: " + std::to_string(prod.get_price()),
                    {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 1, 21, tex);
            }  // PRICE
            {  // MARKET PRICE
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    "Marker price: " + std::to_string(prod.get_market_price()),
                    {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 1, 41, tex);
            }  // MARKET PRICE
            {  // DELTA HEATH
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    "Delta health: " +
                        std::to_string(prod.get_delta_max_health()),
                    {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 1, 61, tex);
            }  // DELTA HEALTH
            {  // DELTA SPEED
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    "Delta speed: " + std::to_string(prod.get_delta_speed()),
                    {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 1, 81, tex);
            }  // DELTA SPEED
            {  // DELTA HAND LIMIT
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    "Delta hand limit: " +
                        std::to_string(prod.get_delta_hand_limit()),
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
                    "    Body: " +
                        std::to_string(chars[::runebound::Characteristic::BODY]
                        ),
                    {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 1, 141, tex);
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    "    Intelligence: " +
                        std::to_string(
                            chars[::runebound::Characteristic::INTELLIGENCE]
                        ),
                    {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 1, 161, tex);
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    "    Spirit: " +
                        std::to_string(
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
                        std::string token_name;
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
                            std::to_string(num), {0x00, 0x00, 0x00, 0xFF}
                        );
                        token_texture.render_to_texture(
                            m_graphic_renderer, 80, 30, token_tex
                        );
                    }  // FACE SIDE
                    {  // BACK SIDE
                        const bool init = token.value().second_lead;
                        const int num = token.value().second_count;
                        std::string token_name;
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
                            std::to_string(num), {0x00, 0x00, 0x00, 0xFF}
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
    m_need_to_update = true;
}
}  // namespace runebound::graphics