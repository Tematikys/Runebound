#include <graphics_client.hpp>
#include <set>

namespace runebound::graphics {
void Client::init_shop_window() {
    auto *win = m_window.get_window("game");
    auto window = std::make_unique<Window>(
        Window(win->width(), win->height(), {0xFF, 0xFF, 0xFF, 0xFF})
    );

    win->add_window("shop", std::move(window), {0, 0}, false, false);
}

void Client::update_shop_window() {
    if (m_network_client.get_yourself_character() == nullptr) {
        std::cout << "[[log]] :: Character does not exist" << std::endl;
        return;
    }

    auto *win = m_window.get_window("game")->get_window("shop");
    win->remove_all_textures();
    for (int i = 1; i <= 4; ++i) {
        win->remove_button(std::to_string(i));
    }

    const auto &pos = m_network_client.get_yourself_character()->get_position();
    if (!m_network_client.get_game_client().m_shops.contains(pos)) {
        std::cout << "[[log]] :: Trade does not exist" << std::endl;
        return;
    }

    static int selected_shop_item = 0;
    static unsigned int selected_shop_item_id = 0;
    if (m_network_client.is_game_need_update()) {
        selected_shop_item = 0;
        selected_shop_item_id = 0;
    }

    // needed, because can not be made in init function
    static bool create_buttons = true;

    {  // UPDATE BUTTONS
        if (create_buttons) {
            create_buttons = false;
            {  // BUY BUTTON
                Texture texture;
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono30"], "Buy",
                    {0x00, 0x00, 0x00, 0xFF}
                );
                Button button(
                    200, 30, HorizontalButtonTextureAlign::CENTER,
                    VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                    [&is_selected = selected_shop_item,
                     &id = selected_shop_item_id, this]() {
                        if (is_selected != 0) {
                            m_network_client.buy_product(id);
                        }
                    },
                    []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
                );
                win->add_button(
                    "Buy", button, {win->width() - 205, win->height() - 35 * 2},
                    true, true
                );
            }  //  BUY BUTTON
            {  // SELL BUTTON
                Texture texture;
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono30"], "Sell",
                    {0x00, 0x00, 0x00, 0xFF}
                );
                Button button(
                    200, 30, HorizontalButtonTextureAlign::CENTER,
                    VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                    [&is_selected = selected_shop_item,
                     &id = selected_shop_item_id, this]() {
                        if (is_selected != 0) {
                            m_network_client.sell_product_in_town(id);
                        }
                    },
                    []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
                );
                win->add_button(
                    "sell", button,
                    {win->width() - 205, win->height() - 35 * 3}, true, true
                );
            }  //  SELL BUTTON
            {  // DISCARD BUTTON
                Texture texture;
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono30"], "Discard",
                    {0x00, 0x00, 0x00, 0xFF}
                );
                Button button(
                    200, 30, HorizontalButtonTextureAlign::CENTER,
                    VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                    [&is_selected = selected_shop_item,
                     &id = selected_shop_item_id, this]() {
                        if (is_selected != 0) {
                            m_network_client.discard_product(id);
                        }
                    },
                    []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
                );
                win->add_button(
                    "discard", button,
                    {win->width() - 205, win->height() - 35 * 4}, true, true
                );
            }  //  DISCARD BUTTON
        }
    }  // UPDATE BUTTONS

    // just for convenience
    static auto comp_text_render = [this](
                                       const std::string &text, Point pos,
                                       SDL_Texture *tex, int size
                                   ) {
        Texture texture;
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono" + std::to_string(size)],
            text, {0x00, 0x00, 0x00, 0xFF}
        );
        texture.render_to_texture(m_graphic_renderer, pos.x(), pos.y(), tex);
    };

    {  // UPDATE PRODUCTS
        int count = 0;
        for (auto id : m_network_client.get_game_client().m_shops.at(pos)) {
            auto product = m_network_client.get_product(id);
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
            const auto name = product.get_product_name();
            int indent = 0;
            {  // NAME
                Texture texture;
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"], name,
                    {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(
                    m_graphic_renderer, 151 - texture.width() / 2,
                    1 + 20 * indent++, tex
                );
            }  // NAME
            comp_text_render(
                "Price: " + std::to_string(product.get_price()),
                {1, 1 + 20 * indent++}, tex, 20
            );
            if (product.get_market_price() != 0) {
                comp_text_render(
                    "Market price: " +
                        std::to_string(product.get_market_price()),
                    {1, 1 + 20 * indent++}, tex, 20
                );
            }
            if (product.get_delta_max_health() != 0) {
                comp_text_render(
                    "Delta health: " +
                        std::to_string(product.get_delta_max_health()),
                    {1, 1 + 20 * indent++}, tex, 20
                );
            }
            if (product.get_delta_speed() != 0) {
                comp_text_render(
                    "Delta speed: " + std::to_string(product.get_delta_speed()),
                    {1, 1 + 20 * indent++}, tex, 20
                );
            }
            if (product.get_delta_hand_limit() != 0) {
                comp_text_render(
                    "Delta hand limit: " +
                        std::to_string(product.get_delta_hand_limit()),
                    {1, 1 + 20 * indent++}, tex, 20
                );
            }
            auto chars = product.get_delta_characteristic();
            const int body = chars[::runebound::Characteristic::BODY];
            const int intelligence =
                chars[::runebound::Characteristic::INTELLIGENCE];
            const int spirit = chars[::runebound::Characteristic::SPIRIT];
            if (body != 0 || intelligence != 0 || spirit != 0) {
                comp_text_render(
                    "Characteristics:", {1, 1 + 20 * indent++}, tex, 20
                );
                if (body != 0) {
                    comp_text_render(
                        "    Body: " + std::to_string(body),
                        {1, 1 + 20 * indent++}, tex, 20
                    );
                }
                if (intelligence != 0) {
                    comp_text_render(
                        "    Intelligence: " + std::to_string(intelligence),
                        {1, 1 + 20 * indent++}, tex, 20
                    );
                }
                if (spirit != 0) {
                    comp_text_render(
                        "    Spirit: " + std::to_string(spirit),
                        {1, 1 + 20 * indent++}, tex, 20
                    );
                }
            }
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
                const auto token = product.get_fight_token();
                if (token) {
                    {  // BACKGROUND
                        m_images["fight_token"].render_to_texture(
                            m_graphic_renderer, 0, 0, token_tex
                        );
                    }  // BACKGROUND
                    {  // FACE SIDE
                        const bool init = token.value().first_lead;
                        const int num = token.value().first_count;
                        std::string token_name =
                            HAND_FIGHT_TOKENS_TO_STR.at(token.value().first);
                        if (init) {
                            token_name += "_init";
                        }
                        m_images[token_name].render_to_texture(
                            m_graphic_renderer, 20, 20, token_tex
                        );
                        comp_text_render(
                            std::to_string(num), {80, 30}, token_tex, 40
                        );
                    }  // FACE SIDE
                    {  // BACK SIDE
                        const bool init = token.value().second_lead;
                        const int num = token.value().second_count;
                        std::string token_name =
                            HAND_FIGHT_TOKENS_TO_STR.at(token.value().second);
                        if (init) {
                            token_name += "_init";
                        }
                        m_images[token_name + "32"].render_to_texture(
                            m_graphic_renderer, 77, 77, token_tex
                        );
                        comp_text_render(
                            std::to_string(num), {75, 100}, token_tex, 20
                        );
                    }  // BACK SIDE
                }
                Texture texture(token_tex);
                texture.render_to_texture(
                    m_graphic_renderer, 1, 1 + 20 * indent, tex
                );
            }  // FIGHT TOKEN
            const int height =
                indent * 20 + (product.get_fight_token() ? 129 : 0);
            auto *res = SDL_CreateTexture(
                m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET, 300, height
            );
            {  // BORDER
                const RectangleShape rect = RectangleShape(0, 0, 299, height);
                rect.render_to_texture(
                    m_graphic_renderer, res, col, {0x00, 0xFF, 0x00, 0xFF}
                );
                Texture temp;
                Button button(
                    300, height, HorizontalButtonTextureAlign::CENTER,
                    VerticalButtonTextureAlign::CENTER, 0, 0, temp,
                    [&num = selected_shop_item, &num_id = selected_shop_item_id,
                     id, count, this]() {
                        if (num == count + 1) {
                            num = 0;
                            num_id = 0;
                        } else {
                            num = count + 1;
                            num_id = id;
                        }
                        m_need_to_update = true;
                    },
                    []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
                );
                win->add_button(
                    std::to_string(count + 1), button, {5 + 305 * count, 5},
                    false, true
                );
            }  // BORDER
            SDL_SetRenderTarget(m_graphic_renderer, res);
            const SDL_Rect clip{0, 0, 300, height};
            SDL_RenderCopy(m_graphic_renderer, tex, &clip, nullptr);
            Texture texture(res);
            win->add_texture(name, texture, {5 + 305 * count, 5}, true);
            SDL_DestroyTexture(tex);
            ++count;
        }
    }  // UPDATE PRODUCTS
}
}  // namespace runebound::graphics