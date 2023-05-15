#include <fight_token.hpp>
#include <graphics_client.hpp>

namespace runebound::graphics {
void Client::init_fight_window() {
    Texture texture;
    Button button;

    auto window = ::std::make_unique<Window>(Window(
        m_graphic_renderer, WINDOW_WIDTH * 3 / 4, WINDOW_HEIGHT * 3 / 4,
        {0xFF, 0xFF, 0xFF, 0xFF}
    ));

    // ===== THROW TOKENS BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Throw tokens",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        12 * 30 * 3 / 5, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() { m_network_client.fight_start_round(); }, []() {},
        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button(
        "throw", button,
        {WINDOW_WIDTH * 3 / 4 - 12 * 30 * 3 / 5 - 5,
         WINDOW_HEIGHT * 3 / 4 - 35 * 2},
        true, true
    );
    // ===== THROW TOKENS BUTTON =====

    // ===== CLOSE BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Exit",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        10 * 30 * 3 / 5, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() {
            m_window.get_window("game")->get_window("fight")->deactivate();
            m_window.get_window("game")->set_visibility_window("fight", false);
            m_window.get_window("game")->set_updatability_window(
                "fight", false
            );
        },
        []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button(
        "exit", button,
        {WINDOW_WIDTH * 3 / 4 - 10 * 30 * 3 / 5 - 5,
         WINDOW_HEIGHT * 3 / 4 - 35 * 1},
        true, true
    );
    // ===== CLOSE BUTTON =====

    m_window.get_window("game")->add_window(
        "fight", ::std::move(window), {WINDOW_WIDTH / 8, WINDOW_HEIGHT / 8},
        false, false
    );
}

void Client::update_fight_window() {
    const auto rand_char = m_network_client.get_game_client().m_characters[0];
    auto tokens = rand_char.get_fight_token();

    auto win = m_window.get_window("game")->get_window("fight");
    win->remove_all_textures();

    const auto &fight = m_network_client.get_game_client().m_fight_client;
    int count = 0;
    for (const auto &token : fight.m_character_remaining_tokens) {
        bool init;
        if (token.token.first == token.hand) {
            init = token.token.first_lead;
        } else {
            init = token.token.second_lead;
        }
        ::std::string name;
        switch (token.hand) {
            case fight::HandFightTokens::PHYSICAL_DAMAGE:
                name = "damage";
                break;
            case fight::HandFightTokens::MAGICAL_DAMAGE:
                name = "magic";
                break;
            case fight::HandFightTokens::DEXTERITY:
                break;
            case fight::HandFightTokens::HIT:
                break;
            case fight::HandFightTokens::ENEMY_DAMAGE:
                break;
            case fight::HandFightTokens::DOUBLING:
                break;
            case fight::HandFightTokens::SHIELD:
                name = "shield";
                break;
            case fight::HandFightTokens::NOTHING:
                name = "none";
                break;
        };
        if (init) {
            name += "_init";
        }
        SDL_Texture *tex = SDL_CreateTexture(
            m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, 32, 32
        );
        SDL_SetRenderTarget(m_graphic_renderer, tex);
        SDL_SetRenderDrawColor(m_graphic_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(m_graphic_renderer);
        SDL_SetRenderTarget(m_graphic_renderer, nullptr);
        m_images[name].render_to_texture(m_graphic_renderer, 0, 0, tex);
        Texture texture(tex);
        win->add_texture(
            ::std::to_string(count), texture, {5 + 69 * count, 5}, true
        );
        ++count;
    }
}
}  // namespace runebound::graphics