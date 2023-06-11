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

    static auto comp_text_render = [this](
                                       const std::string &text, Point pos,
                                       SDL_Texture *tex
                                   ) {
        static Texture texture;
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono20"], text,
            {0x00, 0x00, 0x00, 0xFF}
        );
        texture.render_to_texture(m_graphic_renderer, pos.x(), pos.y(), tex);
    };

    auto *win = m_window.get_window("game")->get_window("inventory");
    win->remove_all_textures();

    auto prods = m_network_client.get_yourself_character()->get_products();
    {  // UPDATE PRODUCTS
        int count = 0;
        for (auto e : prods) {
            auto prod = m_network_client.get_product(e);
            SDL_Texture *tex = SDL_CreateTexture(
                m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET, 300, 330
            );
            const SDL_Color col = {0xFF, 0xFF, 0xFF, 0xFF};
            SDL_SetRenderTarget(m_graphic_renderer, tex);
            SDL_SetRenderDrawColor(
                m_graphic_renderer, col.r, col.g, col.b, col.a
            );
            SDL_RenderClear(m_graphic_renderer);
            SDL_SetRenderTarget(m_graphic_renderer, nullptr);
            Texture texture;
            const RectangleShape rect = RectangleShape(0, 0, 299, 329);
            rect.render_to_texture(
                m_graphic_renderer, tex, col, {0x00, 0xFF, 0x00, 0xFF}
            );
            const auto name = prod.get_product_name();
            comp_text_render(name, {101 - texture.width() / 2, 1}, tex);
            comp_text_render(
                "Price: " + std::to_string(prod.get_price()), {1, 21}, tex
            );
            comp_text_render(
                "Marker price: " + std::to_string(prod.get_market_price()),
                {1, 41}, tex
            );
            comp_text_render(
                "Delta health: " + std::to_string(prod.get_delta_max_health()),
                {1, 61}, tex
            );
            comp_text_render(
                "Delta speed: " + std::to_string(prod.get_delta_speed()),
                {1, 81}, tex
            );
            comp_text_render(
                "Delta hand limit: " +
                    std::to_string(prod.get_delta_hand_limit()),
                {1, 101}, tex
            );
            comp_text_render("Characteristics:", {1, 121}, tex);
            auto chars = prod.get_delta_characteristic();
            comp_text_render(
                "    Body: " +
                    std::to_string(chars[::runebound::Characteristic::BODY]),
                {1, 141}, tex
            );
            comp_text_render(
                "    Intelligence: " +
                    std::to_string(
                        chars[::runebound::Characteristic::INTELLIGENCE]
                    ),
                {1, 161}, tex
            );
            comp_text_render(
                "    Spirit: " +
                    std::to_string(
                        chars[::runebound::Characteristic::INTELLIGENCE]
                    ),
                {1, 181}, tex
            );
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
                        std::string token_name =
                            HAND_FIGHT_TOKENS_TO_STR.at(token.value().first);
                        if (init) {
                            token_name += "_init";
                        }
                        m_images[token_name].render_to_texture(
                            m_graphic_renderer, 20, 20, token_tex
                        );
                        comp_text_render(
                            std::to_string(num), {80, 30}, token_tex
                        );
                    }  // FACE SIDE
                    {  // BACK SIDE
                        const bool init = token.value().second_lead;
                        const int num = token.value().second_count;
                        std::string token_name =
                            HAND_FIGHT_TOKENS_TO_STR.at(token.value().first);
                        if (init) {
                            token_name += "_init";
                        }
                        m_images[token_name + "32"].render_to_texture(
                            m_graphic_renderer, 77, 77, token_tex
                        );
                        comp_text_render(
                            std::to_string(num), {75, 100}, token_tex
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
    {  // UPDATE MEETINGS
        int count = 0;
        for (auto id : m_network_client.get_yourself_character()->get_cards(
                 ::runebound::AdventureType::MEETING
             )) {
            auto card =
                m_network_client.get_game_client().m_all_cards_meeting[id];
            SDL_Texture *tex = SDL_CreateTexture(
                m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_TARGET, 300, 120
            );
            const SDL_Color col = {0xFF, 0xFF, 0xFF, 0xFF};
            SDL_SetRenderTarget(m_graphic_renderer, tex);
            SDL_SetRenderDrawColor(
                m_graphic_renderer, col.r, col.g, col.b, col.a
            );
            SDL_RenderClear(m_graphic_renderer);
            SDL_SetRenderTarget(m_graphic_renderer, nullptr);
            Texture texture;
            const RectangleShape rect = RectangleShape(0, 0, 299, 119);
            rect.render_to_texture(
                m_graphic_renderer, tex, col, {0x00, 0xFF, 0x00, 0xFF}
            );
            const auto name = card.get_name();
            texture.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono20"], name,
                {0x00, 0x00, 0x00, 0xFF}
            );
            texture.render_to_texture(
                m_graphic_renderer, 151 - texture.width() / 2, 1, tex
            );
            comp_text_render("            |First|Second", {1, 21}, tex);
            comp_text_render(
                "Gold        |" +
                    std::to_string(card.get_gold_award(
                        ::runebound::cards::OptionMeeting::FIRST
                    )) +
                    "    |" +
                    std::to_string(card.get_gold_award(
                        ::runebound::cards::OptionMeeting::SECOND
                    )) +
                    "     ",
                {1, 41}, tex
            );
            auto convert = [](runebound::Characteristic c) {
                switch (c) {
                    case runebound::Characteristic::BODY:
                        return std::string("body ");
                    case runebound::Characteristic::INTELLIGENCE:
                        return std::string("intel");
                    case runebound::Characteristic::SPIRIT:
                        return std::string("spir ");
                }
            };
            comp_text_render(
                "Characteris.|" +
                    convert(card.get_verifiable_characteristic(
                        ::runebound::cards::OptionMeeting::FIRST
                    )) +
                    "|" +
                    convert(card.get_verifiable_characteristic(
                        ::runebound::cards::OptionMeeting::SECOND
                    )),
                {1, 61}, tex
            );
            comp_text_render(
                "Delta char. |" +
                    std::to_string(card.get_change_characteristic(
                        ::runebound::cards::OptionMeeting::FIRST
                    )) +
                    "    |" +
                    std::to_string(card.get_change_characteristic(
                        ::runebound::cards::OptionMeeting::SECOND
                    )) +
                    "    ",
                {1, 81}, tex
            );
            comp_text_render(
                "Delta know. |" +
                    std::to_string(card.get_knowledge_token(
                        ::runebound::cards::OptionMeeting::FIRST
                    )) +
                    "    |" +
                    std::to_string(card.get_knowledge_token(
                        ::runebound::cards::OptionMeeting::SECOND
                    )) +
                    "     ",
                {1, 101}, tex
            );
            texture = Texture(tex);
            win->add_texture(name, texture, {5 + 305 * count, 5}, true);
            SDL_DestroyTexture(tex);
            texture.free();
            ++count;
        }
    }  // UPDATE MEETINGS
}
}  // namespace runebound::graphics