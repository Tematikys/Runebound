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
    const auto &fight = m_network_client.get_game_client().m_fight_client;
    if (fight.check_end_fight()) {
        m_network_client.fight_end_fight();
    }

    static ::std::vector<::runebound::fight::TokenHandCount>
        character_selected_tokens;
    static ::std::vector<::runebound::fight::TokenHandCount>
        enemy_selected_tokens;

    if (m_network_client.is_game_need_update()) {
        character_selected_tokens.clear();
        enemy_selected_tokens.clear();
    }

    int count = 0;
    std::vector<::runebound::fight::TokenHandCount> your_tokens;
    std::vector<::runebound::fight::TokenHandCount> opponent_tokens;
    ::std::string your_name;
    ::std::string opponent_name;

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
                                char_vec, enemy_vec
                            );
                        } else {
                            ::std::cout << "Invalid State!!!" << ::std::endl;
                        }
                    }
                },
                []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
            );
            win->add_button(
                "use", button,
                {win->width() - button.width() - 5,
                 win->height() - 138 - button.height() - 5},
                true, true
            );
        }
    }  // USE BUTTON

    {  // PASS BUTTON
        if (m_network_client.get_yourself_character() != nullptr) {
            Texture texture;
            Button button;
            texture.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono30"], "Pass",
                {0x00, 0x00, 0x00, 0xFF}
            );
            button = Button(
                200, 30, HorizontalButtonTextureAlign::CENTER,
                VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                [this]() {
                    if (m_network_client.get_yourself_character()->get_state(
                        ) == ::runebound::character::StateCharacter::FIGHT) {
                        m_network_client.fight_pass(
                            ::runebound::fight::Participant::CHARACTER
                        );
                    } else {
                        m_network_client.fight_pass(
                            ::runebound::fight::Participant::ENEMY
                        );
                    }
                },
                []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
            );
            win->add_button(
                "pass", button,
                {win->width() - button.width() - 5,
                 win->height() - 138 - button.height() - 5 - 35},
                true, true
            );
        }
    }  // PASS BUTTON

    bool am_i_char = false;
    ::runebound::character::Character opponent;

    {  // GET YOURSELF AND OPPONENT
        if (m_network_client.get_yourself_character() != nullptr &&
            m_network_client.get_yourself_character()->get_state() ==
                ::runebound::character::StateCharacter::FIGHT) {
            am_i_char = true;
            your_tokens = fight.m_character_remaining_tokens;
            opponent_tokens = fight.m_enemy_remaining_tokens;
            your_name = m_network_client.get_yourself_character()->get_name();
            opponent_name = fight.m_enemy.get_name();
        } else {
            am_i_char = false;
            opponent_tokens = fight.m_character_remaining_tokens;
            your_tokens = fight.m_enemy_remaining_tokens;
            your_name = fight.m_enemy.get_name();
            for (const auto &op : m_network_client.m_game_client.m_characters) {
                if (op.get_state() ==
                    ::runebound::character::StateCharacter::FIGHT) {
                    opponent = op;
                    opponent_name = op.get_name();
                    break;
                }
            }
        }
    }  // GET YOURSELF AND OPPONENT

    {  // TURN
        Texture texture;
        ::std::string turn_name;
        if (fight.get_turn() == ::runebound::fight::Participant::CHARACTER) {
            if (am_i_char) {
                turn_name = your_name;
            } else {
                turn_name = opponent_name;
            }
        } else {
            if (am_i_char) {
                turn_name = opponent_name;
            } else {
                turn_name = your_name;
            }
        }
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"], "Turn: " + turn_name,
            {0x00, 0x00, 0x00, 0xFF}
        );
        win->add_texture(
            "turn", texture,
            {win->width() - texture.width() - 5,
             win->height() - 138 - texture.height() - 5 - 2 * 35},
            true
        );
    }  // TURN

    {  // YOUR NAME
        Texture texture;
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"], your_name,
            {0x00, 0x00, 0x00, 0xFF}
        );
        win->add_texture("char_name", texture, {5, 138}, true);
    }  // YOUR NAME

    {  // YOUR HEART
        SDL_Texture *tex = SDL_CreateTexture(
            m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, 20, 20
        );
        SDL_SetRenderTarget(m_graphic_renderer, tex);
        SDL_SetRenderDrawColor(m_graphic_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(m_graphic_renderer);
        SDL_SetRenderTarget(m_graphic_renderer, nullptr);
        m_images["heart20"].render_to_texture(m_graphic_renderer, 0, 0, tex);
        Texture texture = Texture(tex);
        win->add_texture("char_heart", texture, {5, 173}, true);
        ::std::string health;
        if (am_i_char) {
            health = ::std::to_string(
                m_network_client.get_yourself_character()->get_health()
            );
        } else {
            health = ::std::to_string(fight.m_enemy.get_health());
        }
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono20"], health,
            {0x00, 0x00, 0x00, 0xFF}
        );
        win->add_texture("char_health", texture, {30, 173}, true);
    }  // YOUR HEART

    {  // ENEMY HEART
        SDL_Texture *tex = SDL_CreateTexture(
            m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, 20, 20
        );
        SDL_SetRenderTarget(m_graphic_renderer, tex);
        SDL_SetRenderDrawColor(m_graphic_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(m_graphic_renderer);
        SDL_SetRenderTarget(m_graphic_renderer, nullptr);
        m_images["heart20"].render_to_texture(m_graphic_renderer, 0, 0, tex);
        Texture texture = Texture(tex);
        win->add_texture(
            "enemy_heart", texture,
            {5, win->height() - 138 - texture.height() - 5 - 35}, true
        );
        ::std::string health;
        if (am_i_char) {
            health = ::std::to_string(fight.m_enemy.get_health());
        } else {
            health = ::std::to_string(opponent.get_health());
        }
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono20"], health,
            {0x00, 0x00, 0x00, 0xFF}
        );
        win->add_texture(
            "enemy_heath", texture,
            {30, win->height() - 138 - texture.height() - 5 - 35}, true
        );
    }  // ENEMY HEART

    {  // ENEMY NAME
        Texture texture;
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"], opponent_name,
            {0x00, 0x00, 0x00, 0xFF}
        );
        win->add_texture(
            "op_name", texture, {5, win->height() - 138 - texture.height() - 5},
            true
        );
    }  // ENEMY NAME

    for (const auto &token : your_tokens) {  // YOUR TOKENS
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
                int num = 0;
                if (token.token.first == token.hand) {
                    init = token.token.first_lead;
                    num = token.token.first_count;
                } else {
                    init = token.token.second_lead;
                    num = token.token.second_count;
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
                }
                if (init) {
                    name += "_init";
                }
                m_images[name].render_to_texture(
                    m_graphic_renderer, 20, 20, tex
                );
                Texture texture;
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono40"],
                    ::std::to_string(num), {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 80, 30, tex);
            }  // FACE SIDE
            {  // BACK SIDE
                bool init = false;
                int num = 0;
                ::std::string name;
                ::runebound::fight::HandFightTokens target{};
                if (token.token.first == token.hand) {
                    target = token.token.second;
                    num = token.token.second_count;
                    init = token.token.second_lead;
                } else {
                    num = token.token.first_count;
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
                }
                if (init) {
                    name += "_init";
                }
                m_images[name + "32"].render_to_texture(
                    m_graphic_renderer, 77, 77, tex
                );
                Texture texture;
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    ::std::to_string(num), {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 75, 100, tex);
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
    }  // YOUR TOKENS

    count = 0;
    for (const auto &token : opponent_tokens) {  // ENEMY TOKENS
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
                int num = 0;
                if (token.token.first == token.hand) {
                    init = token.token.first_lead;
                    num = token.token.first_count;
                } else {
                    init = token.token.second_lead;
                    num = token.token.second_count;
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
                }
                if (init) {
                    name += "_init";
                }
                m_images[name].render_to_texture(
                    m_graphic_renderer, 20, 20, tex
                );
                Texture texture;
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono40"],
                    ::std::to_string(num), {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 80, 30, tex);
            }  // FACE SIDE
            {  // BACK SIDE
                bool init = false;
                int num = 0;
                ::std::string name;
                ::runebound::fight::HandFightTokens target{};
                if (token.token.first == token.hand) {
                    target = token.token.second;
                    num = token.token.second_count;
                    init = token.token.second_lead;
                } else {
                    num = token.token.first_count;
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
                }
                if (init) {
                    name += "_init";
                }
                m_images[name + "32"].render_to_texture(
                    m_graphic_renderer, 77, 77, tex
                );
                Texture texture;
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"],
                    ::std::to_string(num), {0x00, 0x00, 0x00, 0xFF}
                );
                texture.render_to_texture(m_graphic_renderer, 75, 100, tex);
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