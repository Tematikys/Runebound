#include <graphics_client.hpp>

namespace runebound::graphics {
void Client::init_game_window() {
    auto window = ::std::make_unique<Window>(Window(
        m_graphic_renderer, WINDOW_WIDTH, WINDOW_HEIGHT,
        {0xFF, 0xFF, 0xFF, 0xFF}
    ));
    Texture texture;
    Button button;

    // ===== THROW DICE BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Throw dice",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        10 * 30 * 3 / 5, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() { m_network_client.throw_move_dice(); }, []() {},
        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button(
        "throw_dice", button,
        {WINDOW_WIDTH - 10 * 30 * 3 / 5 - 5, WINDOW_HEIGHT - 35 * 5}, true, true
    );
    // ===== THROW DICE BUTTON =====

    // ===== RELAX BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Relax",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        10 * 30 * 3 / 5, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() { m_network_client.relax(); }, []() {},
        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button(
        "relax", button,
        {WINDOW_WIDTH - 10 * 30 * 3 / 5 - 5, WINDOW_HEIGHT - 35 * 4}, true, true
    );
    // ===== RELAX BUTTON =====

    // ===== PASS BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Pass",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        10 * 30 * 3 / 5, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() { m_network_client.pass(); }, []() {},
        {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button(
        "pass", button,
        {WINDOW_WIDTH - 10 * 30 * 3 / 5 - 5, WINDOW_HEIGHT - 35 * 3}, true, true
    );
    // ===== PASS BUTTON =====

    // ===== MAIN MENU BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Main menu",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        10 * 30 * 3 / 5, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() {
            m_network_client.exit_game();
            m_window.set_updatability_window("game", false);
            m_window.set_visibility_window("game", false);
            m_window.set_active_window("main_menu");
            m_window.set_updatability_window("main_menu", true);
            m_window.set_visibility_window("main_menu", true);
        },
        []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button(
        "main_menu", button,
        {WINDOW_WIDTH - 10 * 30 * 3 / 5 - 5, WINDOW_HEIGHT - 35 * 2}, true, true
    );
    // ===== MAIN MENU BUTTON =====

    // ===== EXIT BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Exit",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        10 * 30 * 3 / 5, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() {
            m_network_client.exit_game();
            m_is_running = false;
        },
        []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button(
        "exit", button,
        {WINDOW_WIDTH - 10 * 30 * 3 / 5 - 5, WINDOW_HEIGHT - 35 * 1}, true, true
    );
    // ===== EXIT BUTTON =====

    m_window.add_window("game", ::std::move(window), {0, 0}, false, false);

    // CHARACTERS WINDOW
    // VVVVVVVVVVVV

    window = ::std::make_unique<Window>(Window(
        m_graphic_renderer, 10 * 30 * 3 / 5, 385, {0xFF, 0xFF, 0xFF, 0xFF}
    ));

    m_window.get_window("game")->add_window(
        "chars", ::std::move(window), {WINDOW_WIDTH - 10 * 30 * 3 / 5 - 5, 5},
        true, true
    );

    // AAAAAAAAAAAAAAAAA
    // CHARACTERS WINDOW
}

void Client::update_game_window() {
    auto *win = m_window.get_window("game");

    SDL_Texture *tex = nullptr;
    Texture texture;

    {  // BOARD
        update_board();
        m_board.update_selection(m_mouse_pos - m_board_pos);
        m_board.render_to_texture(m_graphic_renderer, tex);
        for (const auto &character :
             m_network_client.get_game_client().m_characters) {
            const Point center = get_center_of_hexagon(
                character.get_position().x, character.get_position().y
            );
            const ::std::string name = character.get_name();
            m_images[name].render_to_texture(
                m_graphic_renderer, -m_images[name].width() / 2 + center.x(),
                -m_images[name].height() / 2 + center.y(), tex
            );
        }
        texture = Texture(tex);
        win->remove_texture("board");
        win->add_texture("board", texture, m_board_pos, true);
        SDL_DestroyTexture(tex);
        texture.free();
    }  // BOARD

    {  // DICES
        const int size = 50;
        const int delay = 10;
        const int amount =
            static_cast<int>(m_network_client.get_last_dice_result().size());
        tex = SDL_CreateTexture(
            m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, size + 1,
            (size + delay) * amount - delay + 1
        );
        SDL_SetRenderTarget(m_graphic_renderer, tex);
        SDL_SetRenderDrawColor(m_graphic_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(m_graphic_renderer);
        SDL_SetRenderTarget(m_graphic_renderer, nullptr);
        int dx = 0;
        for (const auto &dice : m_network_client.get_last_dice_result()) {
            ::std::vector<Point> vertexes;
            PolygonShape tri;
            auto [key, col] = *DICE_COLOR.find(dice);
            vertexes.emplace_back(size, (size + delay) * dx);
            vertexes.emplace_back(0, size + (size + delay) * dx);
            vertexes.emplace_back(0, (size + delay) * dx);
            tri = PolygonShape{vertexes};
            tri.render_to_texture(
                m_graphic_renderer, tex, col.first, {0x00, 0x00, 0x00, 0xFF}
            );
            vertexes.pop_back();
            vertexes.emplace_back(size, size + (size + delay) * dx);
            tri = PolygonShape{vertexes};
            tri.render_to_texture(
                m_graphic_renderer, tex, col.second, {0x00, 0x00, 0x00, 0xFF}
            );
            dx += 1;
        }
        texture = Texture(tex);
        win->remove_texture("dice");
        win->add_texture(
            "dice", texture,
            {WINDOW_WIDTH - 10 * 30 * 3 / 5 - 10 - size,
             WINDOW_HEIGHT - ((size + delay) * amount - delay + 1) - 5},
            true
        );
        SDL_DestroyTexture(tex);
        texture.free();
    }  // DICES

    {  // SELECTED HEXAGON
        const ::std::size_t index = m_board.get_selected_hexagon();
        if (index != 0xFFFF && m_mouse_pressed) {
            m_network_client.make_move(
                static_cast<int>(index / ::runebound::map::STANDARD_SIZE),
                static_cast<int>(index % ::runebound::map::STANDARD_SIZE)
            );
            m_mouse_pressed = false;
        }
    }  // SELECTED HEXAGON

    {  // TAKE TOKEN
        win->remove_button("take_token");
        if (m_network_client.m_character !=
            ::runebound::character::StandardCharacter::NONE) {
            const auto me = m_network_client.get_yourself_character();
            const auto &pos = me.get_position();
            const auto &cell =
                m_network_client.get_game_client().m_map.m_map[pos.x][pos.y];

            if (cell.get_token() != ::runebound::AdventureType::NOTHING) {
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono30"], "Take token",
                    {0x00, 0x00, 0x00, 0xFF}
                );
                Button button(
                    10 * 30 * 3 / 5, 30, HorizontalButtonTextureAlign::CENTER,
                    VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                    [this]() { m_network_client.take_token(); }, []() {},
                    {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
                );
                win->add_button(
                    "take_token", button,
                    {WINDOW_WIDTH - 10 * 30 * 3 / 5 - 5,
                     WINDOW_HEIGHT - 35 * 6},
                    true, true
                );
            }
        } else {
            ::std::cout << "[[graphics log]] :: take token no self"
                        << ::std::endl;
        }
    }  // TAKE TOKEN

    {  // UPDATE CHARACTERS
        win->get_window("chars")->remove_all_textures();

        int counter = 0;
        RectangleShape rect;
        for (const auto &character :
             m_network_client.get_game_client().m_characters) {
            const auto name = character.get_name();
            const auto gold = ::std::to_string(character.get_gold());
            const auto health = ::std::to_string(character.get_health());

            {  // CHARACTER
                tex = SDL_CreateTexture(
                    m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_TARGET, 40, 40
                );
                SDL_SetRenderTarget(m_graphic_renderer, tex);
                SDL_SetRenderDrawColor(
                    m_graphic_renderer, 0xFF, 0xFF, 0xFF, 0xFF
                );
                SDL_RenderClear(m_graphic_renderer);
                SDL_SetRenderTarget(m_graphic_renderer, nullptr);
                m_images[character.get_name() + "40"].render_to_texture(
                    m_graphic_renderer, 0, 0, tex
                );
                texture = Texture(tex);
                win->get_window("chars")->remove_texture(name + "char");
                win->get_window("chars")->add_texture(
                    name + "char", texture,
                    {win->get_window("chars")->width() - 40,
                     (1 + counter) * (20 * 3 + 5) - 45},
                    true
                );
            }  // CHARACTER

            {  // COIN
                tex = SDL_CreateTexture(
                    m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_TARGET, 20, 20
                );
                SDL_SetRenderTarget(m_graphic_renderer, tex);
                SDL_SetRenderDrawColor(
                    m_graphic_renderer, 0xFF, 0xFF, 0xFF, 0xFF
                );
                SDL_RenderClear(m_graphic_renderer);
                SDL_SetRenderTarget(m_graphic_renderer, nullptr);
                m_images["coin20"].render_to_texture(
                    m_graphic_renderer, 0, 0, tex
                );
                texture = Texture(tex);
                win->get_window("chars")->remove_texture(name + "coin");
                win->get_window("chars")->add_texture(
                    name + "coin", texture, {0, 20 + counter * (20 * 3 + 5)},
                    true
                );
            }  // COIN

            {  // HEART
                tex = SDL_CreateTexture(
                    m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_TARGET, 20, 20
                );
                SDL_SetRenderTarget(m_graphic_renderer, tex);
                SDL_SetRenderDrawColor(
                    m_graphic_renderer, 0xFF, 0xFF, 0xFF, 0xFF
                );
                SDL_RenderClear(m_graphic_renderer);
                SDL_SetRenderTarget(m_graphic_renderer, nullptr);
                m_images["heart20"].render_to_texture(
                    m_graphic_renderer, 0, 0, tex
                );
                texture = Texture(tex);
                win->get_window("chars")->remove_texture(name + "heart");
                win->get_window("chars")->add_texture(
                    name + "heart", texture, {0, 40 + counter * (20 * 3 + 5)},
                    true
                );
            }  // HEART

            {  // NAME
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"], name,
                    {0x00, 0x00, 0x00, 0xFF}
                );
                win->get_window("chars")->remove_texture(name);
                win->get_window("chars")->add_texture(
                    name, texture, {0, counter * (20 * 3 + 5)}, true
                );
            }  // NAME

            {  // GOLD NUM
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"], gold,
                    {0x00, 0x00, 0x00, 0xFF}
                );
                win->get_window("chars")->remove_texture(name + gold);
                win->get_window("chars")->add_texture(
                    name + gold, texture, {25, counter * (20 * 3 + 5) + 20},
                    true
                );
            }  // GOLD NUM

            {  // HEALTH NUM
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono20"], health,
                    {0x00, 0x00, 0x00, 0xFF}
                );
                win->get_window("chars")->remove_texture(name + health);
                win->get_window("chars")->add_texture(
                    name + health, texture, {25, counter * (20 * 3 + 5) + 40},
                    true
                );
            }  // HEALTH NUM

            {  // BORDER
                rect = RectangleShape(0, 0, 10 * 30 * 3 / 5, 60);
                tex = SDL_CreateTexture(
                    m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_TARGET, 10 * 30 * 3 / 5 + 1, 60 + 1
                );
                SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
                rect.render_to_texture(
                    m_graphic_renderer, tex, {0xFF, 0xFF, 0xFF, 0xFF},
                    {0x00, 0xFF, 0x00, 0xFF}
                );
                texture = Texture(tex);
                win->get_window("chars")->remove_texture("_" + name + "border");
                win->get_window("chars")->add_texture(
                    " " + name + "border", texture, {0, counter * (20 * 3 + 5)},
                    true
                );
            }  // BORDER

            ++counter;
        }
        SDL_DestroyTexture(tex);
        texture.free();
    }  // UPDATE CHARACTERS

    if (m_network_client.get_game_client().is_fight) {
        win->get_window("fight")->activate();
        win->set_visibility_window("fight", true);
        win->set_updatability_window("fight", true);
        win->set_all_updatability_button(false);
        update_fight_window();
    } else {
        win->get_window("fight")->deactivate();
        win->set_visibility_window("fight", false);
        win->set_updatability_window("fight", false);
        win->set_all_updatability_button(true);
    }
}
}  // namespace runebound::graphics