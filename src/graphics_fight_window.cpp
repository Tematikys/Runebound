#include <fight_token.hpp>
#include <graphics_client.hpp>

namespace runebound::graphics {
void Client::init_fight_window() {
    auto window = ::std::make_unique<Window>(Window(
        m_graphic_renderer, WINDOW_WIDTH * 3 / 4, WINDOW_HEIGHT * 3 / 4,
        {0xFF, 0xFF, 0xFF, 0xFF}
    ));
    m_window.get_window("game")->add_window(
        "fight", ::std::move(window), {WINDOW_WIDTH / 8, WINDOW_HEIGHT / 8},
        false, false
    );
}

void Client::update_fight_window() {
    auto *win = m_window.get_window("game")->get_window("fight");
    win->remove_all_textures();
    win->remove_all_buttons();

    static ::std::vector<::runebound::fight::TokenHandCount>
        character_selected_tokens;
    static ::std::vector<::runebound::fight::TokenHandCount>
        enemy_selected_tokens;

    if (m_network_client.is_game_need_update()) {
        ::std::cout << "[[glog]] :: reset fight token" << ::std::endl;
        character_selected_tokens.clear();
        enemy_selected_tokens.clear();
    }

    {  // USE BUTTON
        if (m_network_client.get_yourself_character() != nullptr) {
            Texture texture;
            Button button;
            texture.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono30"], "Use",
                {0x00, 0x00, 0x00, 0xFF}
            );
            button = Button(
                200, 30, HorizontalButtonTextureAlign::CENTER,
                VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                [this, &char_vec = character_selected_tokens,
                 &enemy_vec = enemy_selected_tokens]() {
                    std::cout
                        << "Char use: " << char_vec.size()
                        << " Enemy use: " << enemy_vec.size() << " total char: "
                        << m_network_client.get_game_client()
                               .m_fight_client.m_character_remaining_tokens
                               .size()
                        << " total enemy: "
                        << m_network_client.get_game_client()
                               .m_fight_client.m_enemy_remaining_tokens.size()
                        << std::endl;
                    if (m_network_client.get_yourself_character()->get_state(
                        ) == ::runebound::character::StateCharacter::FIGHT) {
                        m_network_client.fight_make(
                            ::runebound::fight::Participant::CHARACTER,
                            char_vec, enemy_vec
                        );
                    } else {
                        if (m_network_client.get_yourself_character()
                                ->get_state() ==
                            ::runebound::character::StateCharacter::ENEMY) {
                            m_network_client.fight_make(
                                ::runebound::fight::Participant::ENEMY,
                                enemy_vec, char_vec
                            );
                        } else {
                            ::std::cout << "Invalid State\n" << ::std::endl;
                        }
                    }
                },
                []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
            );
            win->add_button(
                "use", button,
                {win->width() - button.width() - 5, win->height() - 35}, true,
                true
            );
        }
    }  // USE BUTTON

    const auto &fight = m_network_client.get_game_client().m_fight_client;
    int count = 0;
    for (const auto &token :
         fight.m_character_remaining_tokens) {  // CHARACTER TOKENS
        SDL_Texture *tex = SDL_CreateTexture(
            m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, 128, 128
        );
        SDL_SetRenderTarget(m_graphic_renderer, tex);
        SDL_SetRenderDrawColor(m_graphic_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(m_graphic_renderer);
        SDL_SetRenderTarget(m_graphic_renderer, nullptr);
        {      // RENDER
            {  // BACKGROUND
                if (::std::find(
                        character_selected_tokens.begin(),
                        character_selected_tokens.end(), token
                    ) == character_selected_tokens.end()) {
                    m_images["fight_token"].render_to_texture(
                        m_graphic_renderer, 0, 0, tex
                    );
                } else {
                    m_images["fight_token_selected"].render_to_texture(
                        m_graphic_renderer, 0, 0, tex
                    );
                }
            }  // BACKGROUND
            {  // FACE SIDE
                bool init = false;
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
                        name = "dexterity";
                        break;
                    case fight::HandFightTokens::HIT:
                        name = "hit";
                        break;
                    case fight::HandFightTokens::ENEMY_DAMAGE:
                        name = "skull";
                        break;
                    case fight::HandFightTokens::DOUBLING:
                        name = "double";
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
                m_images[name].render_to_texture(
                    m_graphic_renderer, 20, 20, tex
                );
            }  // FACE SIDE
            {  // BACK SIDE
                bool init = false;
                ::std::string name;
                ::runebound::fight::HandFightTokens target{};
                if (token.token.first == token.hand) {
                    target = token.token.second;
                    init = token.token.second_lead;
                } else {
                    target = token.token.first;
                    init = token.token.first_lead;
                }
                switch (target) {
                    case fight::HandFightTokens::PHYSICAL_DAMAGE:
                        name = "damage";
                        break;
                    case fight::HandFightTokens::MAGICAL_DAMAGE:
                        name = "magic";
                        break;
                    case fight::HandFightTokens::DEXTERITY:
                        name = "dexterity";
                        break;
                    case fight::HandFightTokens::HIT:
                        name = "hit";
                        break;
                    case fight::HandFightTokens::ENEMY_DAMAGE:
                        name = "skull";
                        break;
                    case fight::HandFightTokens::DOUBLING:
                        name = "double";
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
                m_images[name + "32"].render_to_texture(
                    m_graphic_renderer, 77, 77, tex
                );
            }  // BACK SIDE
        }      // RENDER
        Texture texture(tex);
        Button button(
            128, 128, HorizontalButtonTextureAlign::NONE,
            VerticalButtonTextureAlign::NONE, 0, 0, texture,
            [&vec = character_selected_tokens, token]() {
                auto pos = ::std::find(vec.begin(), vec.end(), token);
                if (pos == vec.end()) {
                    vec.push_back(token);
                } else {
                    vec.erase(pos);
                }
            },
            []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0xFF}
        );
        win->add_button(
            "char" + ::std::to_string(count), button, {5 + 133 * count, 5},
            true, true
        );
        ++count;
    }  // CHARACTER TOKENS

    count = 0;
    for (const auto &token : fight.m_enemy_remaining_tokens) {  // ENEMY TOKENS
        SDL_Texture *tex = SDL_CreateTexture(
            m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, 128, 128
        );
        SDL_SetRenderTarget(m_graphic_renderer, tex);
        SDL_SetRenderDrawColor(m_graphic_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(m_graphic_renderer);
        SDL_SetRenderTarget(m_graphic_renderer, nullptr);
        {      // REDNER
            {  // BACKGROUND
                if (::std::find(
                        enemy_selected_tokens.begin(),
                        enemy_selected_tokens.end(), token
                    ) == enemy_selected_tokens.end()) {
                    m_images["fight_token"].render_to_texture(
                        m_graphic_renderer, 0, 0, tex
                    );
                } else {
                    m_images["fight_token_selected"].render_to_texture(
                        m_graphic_renderer, 0, 0, tex
                    );
                }
            }  // BACKGROUND
            {  // FACE SIDE
                bool init = false;
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
                        name = "dexterity";
                        break;
                    case fight::HandFightTokens::HIT:
                        name = "hit";
                        break;
                    case fight::HandFightTokens::ENEMY_DAMAGE:
                        name = "skull";
                        break;
                    case fight::HandFightTokens::DOUBLING:
                        name = "double";
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
                m_images[name].render_to_texture(
                    m_graphic_renderer, 20, 20, tex
                );
            }  // FACE SIDE
            {  // BACK SIDE
                bool init = false;
                ::std::string name;
                ::runebound::fight::HandFightTokens target{};
                if (token.token.first == token.hand) {
                    target = token.token.second;
                    init = token.token.second_lead;
                } else {
                    target = token.token.first;
                    init = token.token.first_lead;
                }
                switch (target) {
                    case fight::HandFightTokens::PHYSICAL_DAMAGE:
                        name = "damage";
                        break;
                    case fight::HandFightTokens::MAGICAL_DAMAGE:
                        name = "magic";
                        break;
                    case fight::HandFightTokens::DEXTERITY:
                        name = "dexterity";
                        break;
                    case fight::HandFightTokens::HIT:
                        name = "hit";
                        break;
                    case fight::HandFightTokens::ENEMY_DAMAGE:
                        name = "skull";
                        break;
                    case fight::HandFightTokens::DOUBLING:
                        name = "double";
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
                m_images[name + "32"].render_to_texture(
                    m_graphic_renderer, 77, 77, tex
                );
            }  // BACK SIDE
        }      // RENDER
        Texture texture(tex);
        Button button(
            128, 128, HorizontalButtonTextureAlign::NONE,
            VerticalButtonTextureAlign::NONE, 0, 0, texture,
            [&vec = enemy_selected_tokens, token]() {
                auto pos = ::std::find(vec.begin(), vec.end(), token);
                if (pos == vec.end()) {
                    vec.push_back(token);
                } else {
                    vec.erase(pos);
                }
            },
            []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0xFF}
        );
        win->add_button(
            "enemy" + ::std::to_string(count), button,
            {5 + 133 * count, win->height() - 133}, true, true
        );
        ++count;
    }  // ENEMY TOKENS
}
}  // namespace runebound::graphics