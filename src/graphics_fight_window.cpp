#include <fight_token.hpp>
#include <graphics_client.hpp>

namespace runebound::graphics {
void Client::init_fight_window() {
    {  // FIGHT WINDOW
        auto window = ::std::make_unique<Window>(Window(
            m_graphic_renderer, WINDOW_WIDTH * 3 / 4, WINDOW_HEIGHT * 3 / 4,
            {0xFF, 0xFF, 0xFF, 0xFF}
        ));
        m_window.get_window("game")->add_window(
            "fight", ::std::move(window), {WINDOW_WIDTH / 8, WINDOW_HEIGHT / 8},
            false, false
        );
    }  // FIGHT WINDOW
    {  // WIN / LOSE WINDOW
        auto *win = m_window.get_window("game")->get_window("fight");
        auto window = ::std::make_unique<Window>(Window(
            m_graphic_renderer, win->width(), win->height(),
            {0xFF, 0xFF, 0xFF, 0xFF}
        ));
        win->add_window("win_lose", ::std::move(window), {0, 0}, false, false);
    }  // WIN / LOSE WINDOW
}

void Client::update_fight_window() {
    if (m_network_client.get_yourself_character() == nullptr) {
        return;
    }
    auto *win = m_window.get_window("game")->get_window("fight");
    win->remove_all_textures();
    win->remove_all_buttons();

    static ::std::vector<::runebound::fight::TokenHandCount>
        character_selected_tokens;
    static ::std::vector<::runebound::fight::TokenHandCount>
        enemy_selected_tokens;

    const auto &fight = m_network_client.get_game_client().m_fight_client;
    const auto character = fight.m_character;
    const auto enemy = fight.m_enemy;
    const auto character_tokens = fight.m_character_remaining_tokens;
    const auto enemy_tokens = fight.m_enemy_remaining_tokens;
    const auto character_name = character.get_name();
    const auto enemy_name = enemy.get_name();
    const auto my_role = m_network_client.get_yourself_character()->get_state();

    {  // UPDATE TOKENS
        if (m_network_client.is_game_need_update()) {
            character_selected_tokens.clear();
            enemy_selected_tokens.clear();
        }
    }  // UPDATE TOKENS

    {  // CHECK END OF FIGHT
        if (fight.check_end_fight()) {
            auto winner = m_network_client.get_winner();
            win->get_window("win_lose")->activate();
            win->set_visibility_window("win_lose", true);
            win->set_updatability_window("win_lose", true);
            win->set_all_updatability_button(false);
            if (winner == ::runebound::fight::Participant::CHARACTER) {
                Texture texture;
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono50"],
                    character_name + " win", {0x00, 0x00, 0x00, 0xFF}
                );
                win->get_window("win_lose")
                    ->add_texture(
                        "res", texture,
                        {(win->get_window("win_lose")->width() - texture.width()
                         ) / 2,
                         (win->get_window("win_lose")->height() -
                          texture.height()) /
                                 2 -
                             25},
                        true
                    );
                //                texture.load_text_from_string(
                //                    m_graphic_renderer, m_fonts["FreeMono50"],
                //                    "Reward: ", {0x00, 0x00, 0x00, 0xFF}
                //                );
                //                win->get_window("win_lose")
                //                    ->add_texture(
                //                        "prize", texture,
                //                        {(win->get_window("win_lose")->width()
                //                        - texture.width()
                //                         ) / 2,
                //                         (win->get_window("win_lose")->height()
                //                         -
                //                          texture.height()) /
                //                                 2 +
                //                             25},
                //                        true
                //                    );
            } else {
                Texture texture;
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono50"],
                    enemy_name + " win", {0x00, 0x00, 0x00, 0xFF}
                );
                win->get_window("win_lose")
                    ->add_texture(
                        "res", texture,
                        {(win->get_window("win_lose")->width() - texture.width()
                         ) / 2,
                         (win->get_window("win_lose")->height() -
                          texture.height()) /
                             2},
                        true
                    );
            }
            if (my_role == ::runebound::character::StateCharacter::FIGHT) {
                {  // EXIT BUTTON
                    Texture texture;
                    texture.load_text_from_string(
                        m_graphic_renderer, m_fonts["FreeMono30"], "Exit",
                        {0x00, 0x00, 0x00, 0xFF}
                    );
                    Button button = Button(
                        200, 30, HorizontalButtonTextureAlign::CENTER,
                        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                        [this]() {
                            m_window.get_window("game")
                                ->get_window("fight")
                                ->get_window("win_lose")
                                ->deactivate();
                            m_window.get_window("game")
                                ->get_window("fight")
                                ->get_window("win_lose")
                                ->deactivate_all_window();
                            m_window.get_window("game")
                                ->get_window("fight")
                                ->set_visibility_window("win_lose", false);
                            m_window.get_window("game")
                                ->get_window("fight")
                                ->set_updatability_window("win_lose", false);
                            m_window.get_window("game")
                                ->get_window("fight")
                                ->set_all_updatability_button(true);
                            m_network_client.fight_end_fight();
                        },
                        []() {}, {0xFF, 0xFF, 0xFF, 0xFF},
                        {0x00, 0x00, 0x00, 0xFF}
                    );
                    win->get_window("win_lose")
                        ->add_button(
                            "exit", button,
                            {win->get_window("win_lose")->width() -
                                 button.width() - 5,
                             win->get_window("win_lose")->height() -
                                 button.height() - 5},
                            true, true
                        );
                }  // EXIT BUTTON
            }
            return;
        } else {
            win->get_window("win_lose")->deactivate();
            win->set_visibility_window("win_lose", false);
            win->set_updatability_window("win_lose", false);
            win->set_all_updatability_button(true);
        }
    }  // CHECK END OF FIGHT

    {  // USE BUTTON
        if (my_role != ::runebound::character::StateCharacter::NORMAL_GAME) {
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
                                enemy_vec, char_vec
                            );
                        } else {
                            ::std::cout << "[[log]] :: Invalid state on using "
                                           "fight tokens (fight window)"
                                        << ::std::endl;
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
        if (my_role != ::runebound::character::StateCharacter::NORMAL_GAME) {
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
                        if (m_network_client.get_yourself_character()
                                ->get_state() ==
                            ::runebound::character::StateCharacter::ENEMY) {
                            m_network_client.fight_pass(
                                ::runebound::fight::Participant::ENEMY
                            );
                        } else {
                            ::std::cout
                                << "[[log]] :: Invalid state on passing "
                                   "(fight window)"
                                << ::std::endl;
                        }
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

    {  // TURN
        Texture texture;
        ::std::string turn_name;
        if (fight.get_turn() == ::runebound::fight::Participant::CHARACTER) {
            turn_name = character_name;
        } else {
            turn_name = enemy_name;
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

    {  // CHARACTER NAME
        Texture texture;
        ::std::string text = character_name;
        if (my_role == ::runebound::character::StateCharacter::FIGHT) {
            text = "You are: " + text;
        }
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"], text,
            {0x00, 0x00, 0x00, 0xFF}
        );
        win->add_texture("char_name", texture, {5, 138}, true);
    }  // CHARACTER NAME

    {  // CHARACTER HEART
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
        const auto health = ::std::to_string(character.get_health());
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono20"], health,
            {0x00, 0x00, 0x00, 0xFF}
        );
        win->add_texture("char_health", texture, {30, 173}, true);
    }  // CHARACTER HEART

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
        const auto health = ::std::to_string(fight.m_enemy.get_health());
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
        ::std::string text = enemy_name;
        if (my_role == ::runebound::character::StateCharacter::ENEMY) {
            text = "You are: " + text;
        }
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"], text,
            {0x00, 0x00, 0x00, 0xFF}
        );
        win->add_texture(
            "enemy_name", texture,
            {5, win->height() - 138 - texture.height() - 5}, true
        );
    }  // ENEMY NAME

    int count = 0;
    for (const auto &token : character_tokens) {  // CHARACTER TOKENS
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
                const int num = token.count;
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
                    target = token.token.first;
                    num = token.token.first_count;
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
        Button button;
        if (my_role != ::runebound::character::StateCharacter::NORMAL_GAME) {
            button = Button(
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
        } else {
            win->add_texture(
                "char" + ::std::to_string(count), texture, {5 + 133 * count, 5},
                true
            );
        }
        ++count;
    }  // CHARACTER TOKENS

    count = 0;
    for (const auto &token : enemy_tokens) {  // ENEMY TOKENS
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
                const int num = token.count;
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
                    target = token.token.first;
                    num = token.token.first_count;
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
        Button button;
        if (my_role != ::runebound::character::StateCharacter::NORMAL_GAME) {
            button = Button(
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
        } else {
            win->add_texture(
                "enemy" + ::std::to_string(count), texture,
                {5 + 133 * count, win->height() - 133}, true
            );
        }
        ++count;
    }  // ENEMY TOKENS
}
}  // namespace runebound::graphics