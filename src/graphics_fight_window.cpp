#include <fight_token.hpp>
#include <graphics_client.hpp>

namespace runebound::graphics {
void Client::init_fight_window() {
    {  // FIGHT WINDOW
        auto window = std::make_unique<Window>(Window(
            WINDOW_WIDTH * 3 / 4, WINDOW_HEIGHT * 3 / 4,
            {0xFF, 0xFF, 0xFF, 0xFF}
        ));
        m_window.get_window("game")->add_window(
            "fight", std::move(window), {WINDOW_WIDTH / 8, WINDOW_HEIGHT / 8},
            false, false
        );
    }  // FIGHT WINDOW
    {  // WIN / LOSE WINDOW
        auto *win = m_window.get_window("game")->get_window("fight");
        auto window = std::make_unique<Window>(
            Window(win->width(), win->height(), {0xFF, 0xFF, 0xFF, 0xFF})
        );
        win->add_window("win_lose", std::move(window), {0, 0}, false, false);
    }  // WIN / LOSE WINDOW
}

void Client::fight_window_check_end_of_fight() {
    auto *win = m_window.get_window("game")->get_window("fight");

    const auto &fight = m_network_client.get_game_client().m_fight_client;
    const auto character_name = fight.m_character.get_name();
    const auto enemy_name = fight.m_enemy.get_name();
    const auto my_role = m_network_client.get_yourself_character()->get_state();
    if (fight.check_end_fight()) {
        auto *win_lose =
            m_window.get_window("game")->get_window("fight")->get_window(
                "win_lose"
            );
        win_lose->remove_all_textures();
        win_lose->remove_all_buttons();

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
                    {(win->get_window("win_lose")->width() - texture.width()) /
                         2,
                     (win->get_window("win_lose")->height() - texture.height()
                     ) / 2 -
                         25},
                    true
                );
            texture.free();
            texture.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono50"],
                "Reward: Gold: " +
                    std::to_string(m_network_client.get_game_client()
                                       .m_reward_gold_for_fight) +
                    "!",
                {0x00, 0x00, 0x00, 0xFF}
            );
            win->get_window("win_lose")
                ->add_texture(
                    "prize", texture,
                    {(win->get_window("win_lose")->width() - texture.width()) /
                         2,
                     (win->get_window("win_lose")->height() - texture.height()
                     ) / 2 +
                         25},
                    true
                );
        } else {
            Texture texture;
            texture.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono50"], enemy_name + " win",
                {0x00, 0x00, 0x00, 0xFF}
            );
            win->get_window("win_lose")
                ->add_texture(
                    "res", texture,
                    {(win->get_window("win_lose")->width() - texture.width()) /
                         2,
                     (win->get_window("win_lose")->height() - texture.height()
                     ) / 2},
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
                Button button(
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
                        m_window.get_window("game")
                            ->get_window("fight")
                            ->deactivate();
                        m_window.get_window("game")
                            ->get_window("fight")
                            ->deactivate_all_window();
                        m_window.get_window("game")->set_visibility_window(
                            "fight", false
                        );
                        m_window.get_window("game")->set_updatability_window(
                            "fight", false
                        );
                        m_window.get_window("game")
                            ->set_all_updatability_button(true);
                    },
                    []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
                );
                win->get_window("win_lose")
                    ->add_button(
                        "exit", button,
                        {win->get_window("win_lose")->width() - button.width() -
                             5,
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
}

void Client::fight_window_use_button(
    std::vector<::runebound::fight::TokenHandCount> &character_selected_tokens,
    std::vector<::runebound::fight::TokenHandCount> &enemy_selected_tokens
) {
    auto *win = m_window.get_window("game")->get_window("fight");

    const auto my_role = m_network_client.get_yourself_character()->get_state();
    if (my_role != ::runebound::character::StateCharacter::NORMAL_GAME) {
        Texture texture;
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"], "Use",
            {0x00, 0x00, 0x00, 0xFF}
        );
        Button button(
            200, 30, HorizontalButtonTextureAlign::CENTER,
            VerticalButtonTextureAlign::CENTER, 0, 0, texture,
            [this, &char_vec = character_selected_tokens,
             &enemy_vec = enemy_selected_tokens]() {
                if (m_network_client.get_yourself_character()->get_state() ==
                    ::runebound::character::StateCharacter::FIGHT) {
                    m_network_client.fight_make(
                        ::runebound::fight::Participant::CHARACTER, char_vec,
                        enemy_vec
                    );
                } else {
                    if (m_network_client.get_yourself_character()->get_state(
                        ) == ::runebound::character::StateCharacter::ENEMY) {
                        m_network_client.fight_make(
                            ::runebound::fight::Participant::ENEMY, enemy_vec,
                            char_vec
                        );
                    } else {
                        std::cout << "[[log]] :: Invalid state on using "
                                     "fight tokens (fight window)"
                                  << std::endl;
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
}

void Client::fight_window_pass_button() {
    auto *win = m_window.get_window("game")->get_window("fight");
    const auto my_role = m_network_client.get_yourself_character()->get_state();
    if (my_role != ::runebound::character::StateCharacter::NORMAL_GAME) {
        Texture texture;
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"], "Pass",
            {0x00, 0x00, 0x00, 0xFF}
        );
        Button button(
            200, 30, HorizontalButtonTextureAlign::CENTER,
            VerticalButtonTextureAlign::CENTER, 0, 0, texture,
            [this]() {
                if (m_network_client.get_yourself_character()->get_state() ==
                    ::runebound::character::StateCharacter::FIGHT) {
                    m_network_client.fight_pass(
                        ::runebound::fight::Participant::CHARACTER
                    );
                } else {
                    if (m_network_client.get_yourself_character()->get_state(
                        ) == ::runebound::character::StateCharacter::ENEMY) {
                        m_network_client.fight_pass(
                            ::runebound::fight::Participant::ENEMY
                        );
                    } else {
                        std::cout << "[[log]] :: Invalid state on passing "
                                     "(fight window)"
                                  << std::endl;
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
}

void Client::fight_window_turn() {
    auto *win = m_window.get_window("game")->get_window("fight");
    const auto &fight = m_network_client.get_game_client().m_fight_client;
    const auto character_name = fight.m_character.get_name();
    const auto enemy_name = fight.m_enemy.get_name();
    std::string turn_name;
    if (fight.get_turn() == ::runebound::fight::Participant::CHARACTER) {
        turn_name = character_name;
    } else {
        turn_name = enemy_name;
    }
    Texture texture;
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
}

void Client::fight_window_round_number() {
    auto *win = m_window.get_window("game")->get_window("fight");
    const auto &fight = m_network_client.get_game_client().m_fight_client;
    Texture texture;
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"],
        "Round: " + std::to_string(fight.m_number_of_rounds),
        {0x00, 0x00, 0x00, 0xFF}
    );
    win->add_texture(
        "round", texture,
        {win->width() - texture.width() - 5,
         win->height() - 138 - texture.height() - 5 - 3 * 35},
        true
    );
}

void Client::fight_window_character() {
    auto *win = m_window.get_window("game")->get_window("fight");
    const auto &fight = m_network_client.get_game_client().m_fight_client;
    const auto character_name = fight.m_character.get_name();
    const auto my_role = m_network_client.get_yourself_character()->get_state();
    {  // NAME
        std::string text = character_name;
        if (my_role == ::runebound::character::StateCharacter::FIGHT) {
            text = "You are: " + text;
        }
        Texture texture;
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"], text,
            {0x00, 0x00, 0x00, 0xFF}
        );
        win->add_texture(
            "char_name", texture,
            {5, 138 + 133 * (static_cast<int>(
                                 fight.m_character_remaining_tokens.size()
                             ) /
                             6)},
            true
        );
    }  // NAME
    {  // HEART
        SDL_Texture *tex = SDL_CreateTexture(
            m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, 20, 20
        );
        SDL_SetRenderTarget(m_graphic_renderer, tex);
        SDL_SetRenderDrawColor(m_graphic_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(m_graphic_renderer);
        SDL_SetRenderTarget(m_graphic_renderer, nullptr);
        m_images["heart20"].render_to_texture(m_graphic_renderer, 0, 0, tex);
        Texture texture(tex);
        win->add_texture(
            "char_heart", texture,
            {5, 173 + 133 * (static_cast<int>(
                                 fight.m_character_remaining_tokens.size()
                             ) /
                             6)},
            true
        );
        const auto health = std::to_string(fight.m_character.get_health());
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono20"], health,
            {0x00, 0x00, 0x00, 0xFF}
        );
        win->add_texture(
            "char_health", texture,
            {30, 173 + 133 * (static_cast<int>(
                                  fight.m_character_remaining_tokens.size()
                              ) /
                              6)},
            true
        );
    }  // HEART
}

void Client::fight_window_enemy() {
    auto *win = m_window.get_window("game")->get_window("fight");
    const auto &fight = m_network_client.get_game_client().m_fight_client;
    const auto enemy_name = fight.m_enemy.get_name();
    const auto my_role = m_network_client.get_yourself_character()->get_state();
    {  // HEART
        SDL_Texture *tex = SDL_CreateTexture(
            m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, 20, 20
        );
        SDL_SetRenderTarget(m_graphic_renderer, tex);
        SDL_SetRenderDrawColor(m_graphic_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(m_graphic_renderer);
        SDL_SetRenderTarget(m_graphic_renderer, nullptr);
        m_images["heart20"].render_to_texture(m_graphic_renderer, 0, 0, tex);
        Texture texture(tex);
        win->add_texture(
            "enemy_heart", texture,
            {5, win->height() - 138 - texture.height() - 5 - 35 -
                    133 * (static_cast<int>(fight.m_enemy_remaining_tokens.size(
                           )) /
                           6)},
            true
        );
        const auto health = std::to_string(fight.m_enemy.get_health());
        texture.free();
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono20"], health,
            {0x00, 0x00, 0x00, 0xFF}
        );
        win->add_texture(
            "enemy_heath", texture,
            {30,
             win->height() - 138 - texture.height() - 5 - 35 -
                 133 *
                     (static_cast<int>(fight.m_enemy_remaining_tokens.size()) /
                      6)},
            true
        );
    }  // HEART

    {  // NAME
        std::string text = enemy_name;
        if (my_role == ::runebound::character::StateCharacter::ENEMY) {
            text = "You are: " + text;
        }
        Texture texture;
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"], text,
            {0x00, 0x00, 0x00, 0xFF}
        );
        win->add_texture(
            "enemy_name", texture,
            {5, win->height() - 138 - texture.height() - 5 -
                    133 * (static_cast<int>(fight.m_enemy_remaining_tokens.size(
                           )) /
                           6)},
            true
        );
    }  // NAME
}

void Client::fight_window_generate_texture(
    SDL_Texture *tex,
    ::runebound::fight::TokenHandCount token,
    std::vector<::runebound::fight::TokenHandCount> &vec
) {
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
    SDL_SetRenderTarget(m_graphic_renderer, tex);
    SDL_SetRenderDrawColor(m_graphic_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(m_graphic_renderer);
    SDL_SetRenderTarget(m_graphic_renderer, nullptr);
    {      // RENDER
        {  // BACKGROUND
            if (std::find(vec.begin(), vec.end(), token) == vec.end()) {
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
            std::string name = HAND_FIGHT_TOKENS_TO_STR.at(token.hand);
            if (token.token.first == token.hand) {
                name += token.token.first_lead ? "_init" : "";
            } else {
                name += token.token.second_lead ? "_init" : "";
            }
            m_images[name].render_to_texture(m_graphic_renderer, 20, 20, tex);
            comp_text_render(std::to_string(token.count), {80, 30}, tex, 40);
        }  // FACE SIDE
        {  // BACK SIDE
            int num = 0;
            std::string name;
            if (token.token.first == token.hand) {
                num = token.token.second_count;
                name = HAND_FIGHT_TOKENS_TO_STR.at(token.token.second);
                name += token.token.second_lead ? "_init" : "";
            } else {
                num = token.token.first_count;
                name = HAND_FIGHT_TOKENS_TO_STR.at(token.token.first);
                name += token.token.first_lead ? "_init" : "";
            }
            m_images[name + "32"].render_to_texture(
                m_graphic_renderer, 77, 77, tex
            );
            comp_text_render(std::to_string(num), {75, 100}, tex, 20);
        }  // BACK SIDE
    }      // RENDER
}

void Client::fight_window_tokens(
    bool character,
    std::vector<::runebound::fight::TokenHandCount> &vec
) {
    auto *win = m_window.get_window("game")->get_window("fight");
    const auto &fight = m_network_client.get_game_client().m_fight_client;
    const auto my_role = m_network_client.get_yourself_character()->get_state();
    int count = 0;
    for (const auto &token :
         (character ? fight.m_character_remaining_tokens
                    : fight.m_enemy_remaining_tokens)) {
        SDL_Texture *tex = SDL_CreateTexture(
            m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, 128, 128
        );
        fight_window_generate_texture(tex, token, vec);
        Texture texture(tex);
        if (my_role != ::runebound::character::StateCharacter::NORMAL_GAME) {
            Button button(
                128, 128, HorizontalButtonTextureAlign::NONE,
                VerticalButtonTextureAlign::NONE, 0, 0, texture,
                [&vec = vec, token]() {
                    auto pos = std::find(vec.begin(), vec.end(), token);
                    if (pos == vec.end()) {
                        vec.push_back(token);
                    } else {
                        vec.erase(pos);
                    }
                },
                []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0xFF, 0xFF, 0xFF, 0xFF}
            );
            win->add_button(
                (character ? "char" : "enemy") + std::to_string(count), button,
                (character ? Point(5 + 133 * (count % 6), 5 + 133 * (count / 6))
                           : Point(
                                 5 + 133 * (count % 6),
                                 win->height() - 133 * (1 + count / 6)
                             )),
                true, true
            );
        } else {
            win->add_texture(
                (character ? "char" : "enemy") + std::to_string(count), texture,
                (character ? Point(5 + 133 * (count % 6), 5 + 133 * (count / 6))
                           : Point(
                                 5 + 133 * (count % 6),
                                 win->height() - 133 * (1 + count / 6)
                             )),
                true
            );
        }
        ++count;
    }
}

void Client::update_fight_window() {
    if (m_network_client.get_yourself_character() == nullptr) {
        return;
    }
    auto *win = m_window.get_window("game")->get_window("fight");
    win->remove_all_textures();
    win->remove_all_buttons();

    static std::vector<::runebound::fight::TokenHandCount>
        character_selected_tokens;
    static std::vector<::runebound::fight::TokenHandCount>
        enemy_selected_tokens;

    {  // UPDATE TOKENS
        if (m_network_client.is_game_need_update()) {
            character_selected_tokens.clear();
            enemy_selected_tokens.clear();
        }
    }  // UPDATE TOKENS

    fight_window_check_end_of_fight();
    fight_window_use_button(character_selected_tokens, enemy_selected_tokens);
    fight_window_pass_button();
    fight_window_turn();
    fight_window_round_number();
    fight_window_character();
    fight_window_enemy();
    fight_window_tokens(true, character_selected_tokens);
    fight_window_tokens(false, enemy_selected_tokens);
}
}  // namespace runebound::graphics