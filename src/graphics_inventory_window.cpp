#include <graphics_client.hpp>

namespace runebound::graphics {
void Client::init_inventory_window() {
    auto *win = m_window.get_window("game");
    auto window = std::make_unique<Window>(Window(
        WINDOW_WIDTH * 3 / 4, WINDOW_HEIGHT * 3 / 4, {0xFF, 0xFF, 0xFF, 0xFF}
    ));
    {  // EXIT BUTTON
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
                m_window.get_window("game")
                    ->get_window("inventory")
                    ->deactivate();
                m_window.get_window("game")->set_visibility_window(
                    "inventory", false
                );
                m_window.get_window("game")->set_updatability_window(
                    "inventory", false
                );
                m_window.get_window("game")->set_all_updatability_button(true);
                std::cout << "Exit from inventory" << std::endl;
            },
            []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
        );
        window->add_button(
            "exit", button, {window->width() - 205, window->height() - 35 * 1},
            true, true
        );
    }  // EXIT BUTTON
    win->add_window(
        "inventory", std::move(window), {WINDOW_WIDTH / 8, WINDOW_HEIGHT / 8},
        false, false
    );
}

void Client::update_inventory_window() {
    if (m_network_client.get_yourself_character() == nullptr) {
        return;
    }

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

    auto *win = m_window.get_window("game")->get_window("inventory");
    win->remove_all_textures();
    for (int i = 0; i < 10; ++i) {
        win->remove_button(std::to_string(i) + "first");
        win->remove_button(std::to_string(i) + "second");
    }

    auto prods = m_network_client.get_yourself_character()->get_products();
    {  // UPDATE PRODUCTS
        int count = 0;
        for (auto id : prods) {
            auto product = m_network_client.get_product(id);
            SDL_Texture *tex = SDL_CreateTexture(
                m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET, 300, 330
            );
            SDL_Color col;
            col = {0xFF, 0xFF, 0xFF, 0xFF};
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
    {  // UPDATE MEETINGS
        int count = 0;
        for (auto id : m_network_client.get_yourself_character()->get_cards(
                 ::runebound::AdventureType::MEETING
             )) {
            auto card =
                m_network_client.get_game_client().m_all_cards_meeting[id];
            SDL_Texture *tex = SDL_CreateTexture(
                m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET, 164, 120
            );
            const SDL_Color col = {0xFF, 0xFF, 0xFF, 0xFF};
            SDL_SetRenderTarget(m_graphic_renderer, tex);
            SDL_SetRenderDrawColor(
                m_graphic_renderer, col.r, col.g, col.b, col.a
            );
            SDL_RenderClear(m_graphic_renderer);
            SDL_SetRenderTarget(m_graphic_renderer, nullptr);
            const RectangleShape rect = RectangleShape(0, 0, 163, 119);
            rect.render_to_texture(
                m_graphic_renderer, tex, col, {0x00, 0xFF, 0x00, 0xFF}
            );
            const auto name = card.get_name();
            {
                Texture texture;
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"], name,
                    {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(
                    m_graphic_renderer, 82 - texture.width() / 2, 0, tex
                );
            }
            {  // FIRST
                Texture texture;
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"], "First",
                    {0x00, 0x00, 0x00, 0xFF}
                );
                Button button(
                    texture.width(), texture.height(),
                    HorizontalButtonTextureAlign::CENTER,
                    VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                    [this, id]() {
                        m_network_client.start_card_execution(
                            id, ::runebound::AdventureType::MEETING
                        );
                        m_network_client.check_characteristic(
                            id, ::runebound::cards::OptionMeeting::FIRST
                        );
                    },
                    []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
                );
                win->add_button(
                    std::to_string(count) + "first", button, {26, 355}, true,
                    true
                );
            }  // FIRST
            {  // SECOND
                Texture texture;
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"], "Second",
                    {0x00, 0x00, 0x00, 0xFF}
                );
                Button button(
                    texture.width(), texture.height(),
                    HorizontalButtonTextureAlign::CENTER,
                    VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                    [this, id]() {
                        m_network_client.start_card_execution(
                            id, ::runebound::AdventureType::MEETING
                        );
                        m_network_client.check_characteristic(
                            id, ::runebound::cards::OptionMeeting::SECOND
                        );
                    },
                    []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
                );
                win->add_button(
                    std::to_string(count) + "second", button, {92, 355}, true,
                    true
                );
            }  // SECOND
            m_images["coin20"].render_to_texture(
                m_graphic_renderer, 0, 40, tex
            );
            comp_text_render(
                std::to_string(card.get_gold_award(
                    ::runebound::cards::OptionMeeting::FIRST
                )),
                {50, 40}, tex, 20
            );
            comp_text_render(
                std::to_string(card.get_gold_award(
                    ::runebound::cards::OptionMeeting::SECOND
                )),
                {122, 40}, tex, 20
            );
            static const auto convert = [](runebound::Characteristic c) {
                switch (c) {
                    case runebound::Characteristic::BODY:
                        return std::string("body");
                    case runebound::Characteristic::INTELLIGENCE:
                        return std::string("intelligence");
                    case runebound::Characteristic::SPIRIT:
                        return std::string("spirit");
                    default:
                        return std::string("none_meeting");
                }
            };
            m_images[convert(card.get_verifiable_characteristic(
                         ::runebound::cards::OptionMeeting::FIRST
                     ))]
                .render_to_texture(m_graphic_renderer, 46, 60, tex);
            m_images[convert(card.get_verifiable_characteristic(
                         ::runebound::cards::OptionMeeting::SECOND
                     ))]
                .render_to_texture(m_graphic_renderer, 118, 60, tex);

            m_images["delta"].render_to_texture(m_graphic_renderer, 0, 80, tex);
            comp_text_render(
                std::to_string(card.get_change_characteristic(
                    ::runebound::cards::OptionMeeting::FIRST
                )),
                {50, 80}, tex, 20
            );
            comp_text_render(
                std::to_string(card.get_change_characteristic(
                    ::runebound::cards::OptionMeeting::SECOND
                )),
                {122, 80}, tex, 20
            );

            m_images["knowledge"].render_to_texture(
                m_graphic_renderer, 0, 100, tex
            );
            comp_text_render(
                std::to_string(card.get_knowledge_token(
                    ::runebound::cards::OptionMeeting::FIRST
                )),
                {50, 100}, tex, 20
            );
            comp_text_render(
                std::to_string(card.get_knowledge_token(
                    ::runebound::cards::OptionMeeting::SECOND
                )),
                {122, 100}, tex, 20
            );

            Texture texture(tex);
            win->add_texture(name, texture, {5 + 305 * count, 335}, true);
            ++count;
        }
    }  // UPDATE MEETINGS
}
}  // namespace runebound::graphics