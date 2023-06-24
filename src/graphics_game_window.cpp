#include <graphics_client.hpp>

namespace runebound::graphics {
void Client::init_game_window() {
    {  // GAME WINDOW
        auto *win = &m_window;
        auto window = std::make_unique<Window>(
            Window(win->width(), win->height(), {0xFF, 0xFF, 0xFF, 0xFF})
        );
        {  // THROW DICE BUTTON
            Texture texture;
            texture.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono30"], "Throw dice",
                {0x00, 0x00, 0x00, 0xFF}
            );
            Button button(
                200, 30, HorizontalButtonTextureAlign::CENTER,
                VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                [this]() { m_network_client.throw_move_dice(); }, []() {},
                {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
            );
            window->add_button(
                "throw_dice", button,
                {window->width() - button.width() - 5,
                 window->height() - 35 * 5},
                true, true
            );
        }  // THROW DICE BUTTON
        {  // RELAX BUTTON
            Texture texture;
            texture.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono30"], "Relax",
                {0x00, 0x00, 0x00, 0xFF}
            );
            Button button(
                200, 30, HorizontalButtonTextureAlign::CENTER,
                VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                [this]() { m_network_client.relax(); }, []() {},
                {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
            );
            window->add_button(
                "relax", button,
                {window->width() - button.width() - 5,
                 window->height() - 35 * 4},
                true, true
            );
        }  // RELAX BUTTON
        {  // PASS BUTTON
            Texture texture;
            texture.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono30"], "Pass",
                {0x00, 0x00, 0x00, 0xFF}
            );
            Button button(
                200, 30, HorizontalButtonTextureAlign::CENTER,
                VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                [this]() { m_network_client.pass(); }, []() {},
                {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
            );
            window->add_button(
                "pass", button,
                {window->width() - button.width() - 5,
                 window->height() - 35 * 3},
                true, true
            );
        }  // PASS BUTTON
        {  // MAIN MENU BUTTON
            Texture texture;
            texture.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono30"], "Main menu",
                {0x00, 0x00, 0x00, 0xFF}
            );
            Button button(
                200, 30, HorizontalButtonTextureAlign::CENTER,
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
                {window->width() - button.width() - 5,
                 window->height() - 35 * 2},
                true, true
            );
        }  // MAIN MENU BUTTON
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
                    m_network_client.exit_game();
                    m_is_running = false;
                },
                []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
            );
            window->add_button(
                "exit", button,
                {window->width() - button.width() - 5,
                 window->height() - 35 * 1},
                true, true
            );
        }  // EXIT BUTTON
        win->add_window("game", std::move(window), {0, 0}, false, false);
    }  // GAME WINDOW

    {  // CHARACTERS WINDOW
        auto *win = m_window.get_window("game");
        auto window =
            std::make_unique<Window>(Window(360, 321, {0xFF, 0xFF, 0xFF, 0xFF})
            );
        auto window_width = window->width();
        win->add_window(
            "chars", std::move(window), {win->width() - window_width - 5, 5},
            true, true
        );
    }  // CHARACTERS WINDOW

    {  // PLAYER WINDOW
        auto *win = m_window.get_window("game");
        auto window =
            std::make_unique<Window>(Window(360, 60, {0xFF, 0xFF, 0xFF, 0xFF}));
        auto window_width = window->width();
        auto window_height = window->height();
        win->add_window(
            "player", std::move(window),
            {win->width() - window_width - 265,
             win->height() - window_height - 5},
            true, true
        );
    }  // PLAYER WINDOW

    {  // END WINDOW
        auto *win = m_window.get_window("game");
        auto window = std::make_unique<Window>(
            Window(win->width(), win->height(), {0xFF, 0xFF, 0xFF, 0xFF})
        );
        win->add_window("end", std::move(window), {0, 0}, false, false);
    }  // END WINDOW
}

void Client::update_game_window() {
    if (m_network_client.get_yourself_character() == nullptr) {
        return;
    }

    static auto img_render = [this](
                                 Window *win, const std::string &name,
                                 const std::string &id, Point pos, Point size
                             ) {
        SDL_Texture *tex = SDL_CreateTexture(
            m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET, size.x(), size.y()
        );
        SDL_SetRenderTarget(m_graphic_renderer, tex);
        SDL_SetRenderDrawColor(m_graphic_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(m_graphic_renderer);
        SDL_SetRenderTarget(m_graphic_renderer, nullptr);
        m_images[name].render_to_texture(m_graphic_renderer, 0, 0, tex);
        Texture texture(tex);
        win->remove_texture(id);
        win->add_texture(id, texture, pos, true);
    };
    static auto text_render = [this](
                                  Window *win, const std::string &text,
                                  const std::string &id, Point pos, int size
                              ) {
        Texture texture;
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono" + std::to_string(size)],
            text, {0x00, 0x00, 0x00, 0xFF}
        );
        win->remove_texture(id);
        win->add_texture(id, texture, pos, true);
    };

    {  // BOARD
        SDL_Texture *tex = nullptr;
        auto *window = m_window.get_window("game");
        update_board();
        m_board.update_selection(m_mouse_pos - m_board_pos);
        m_board.render_to_texture(m_graphic_renderer, tex, m_images);
        for (const auto &character :
             m_network_client.get_game_client().m_characters) {
            const Point center = get_center_of_hexagon(
                character.get_position().x, character.get_position().y
            );
            const std::string name =
                CHARACTER_NAMES_WITH_DASH.at(character.get_name());
            m_images[name].render_to_texture(
                m_graphic_renderer, -m_images[name].width() / 2 + center.x(),
                -m_images[name].height() / 2 + center.y(), tex
            );
        }
        Texture texture(tex);
        window->remove_texture("board");
        window->add_texture("board", texture, m_board_pos, true);
    }  // BOARD

    {  // DICES
        SDL_Texture *tex = nullptr;
        auto *window = m_window.get_window("game");
        const int size = 50;
        const int delay = 5;
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
            std::vector<Point> vertexes;
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
        Texture texture(tex);
        window->remove_texture("dice");
        window->add_texture(
            "dice", texture,
            {window->width() - 205 - size - delay,
             window->height() - ((size + delay) * amount - delay + 1) - 5},
            true
        );
    }  // DICES

    {  // SELECTED HEXAGON
        if (m_window.get_window("game")->get_active_window_name().empty()) {
            const std::size_t index = m_board.get_selected_hexagon();
            if (index != 0xFFFF && m_mouse_pressed) {
                m_network_client.make_move(
                    static_cast<int>(index / ::runebound::map::STANDARD_SIZE),
                    static_cast<int>(index % ::runebound::map::STANDARD_SIZE)
                );
                m_mouse_pressed = false;
            }
        }
    }  // SELECTED HEXAGON

    {  // GAME ROUND
        auto *window = m_window.get_window("game");
        Texture texture;
        if (!m_network_client.get_game_client().m_characters.empty()) {
            texture.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono30"],
                "Round: " +
                    std::to_string(
                        m_network_client.m_game_client.m_number_of_rounds
                    ),
                {0x00, 0x00, 0x00, 0xFF}
            );
            window->add_texture(
                "round", texture,
                {window->width() - texture.width(), window->height() - 35 * 10},
                true
            );
        }
    }  // GAME ROUND

    {  // TURN
        auto *window = m_window.get_window("game");
        Texture texture;
        if (!m_network_client.get_game_client().m_characters.empty()) {
            const auto turn_name =
                m_network_client.get_game_client()
                    .m_characters[m_network_client.get_game_client().m_turn]
                    .get_name();
            texture.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono30"], "Turn: " + turn_name,
                {0x00, 0x00, 0x00, 0xFF}
            );
            window->add_texture(
                "turn", texture,
                {window->width() - texture.width(), window->height() - 35 * 9},
                true
            );
        }
    }  // TURN

    {  // ACTION POINTS
        auto *window = m_window.get_window("game");
        if (!m_network_client.get_game_client().m_characters.empty()) {
            Texture texture;
            texture.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono30"],
                "Action Points: " +
                    std::to_string(
                        m_network_client.get_active_character_action_points()
                    ),
                {0x00, 0x00, 0x00, 0xFF}
            );
            window->add_texture(
                "points", texture,
                {window->width() - texture.width(), window->height() - 35 * 8},
                true
            );
        }
    }  // ACTION POINTS

    {  // ADD BOT
        auto *window = m_window.get_window("game");
        window->remove_button("add_bot");
        Texture texture;
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"], "Add bot",
            {0x00, 0x00, 0x00, 0xFF}
        );
        Button button(
            200, 30, HorizontalButtonTextureAlign::CENTER,
            VerticalButtonTextureAlign::CENTER, 0, 0, texture,
            [this]() { m_network_client.add_bot(); }, []() {},
            {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
        );
        window->add_button(
            "add_bot", button,
            {window->width() - 2 * 205, window->height() - 35 * 7}, true, true
        );
    }  // ADD BOT

    {  // TRADE
        auto *window = m_window.get_window("game");
        window->remove_button("trade");
        const auto *me = m_network_client.get_yourself_character();
        const auto &pos = me->get_position();
        const auto &cell =
            m_network_client.get_game_client().m_map.m_map[pos.x][pos.y];
        if (cell.get_type_cell() == ::runebound::map::TypeCell::TOWN) {
            Texture texture;
            texture.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono30"], "Trade",
                {0x00, 0x00, 0x00, 0xFF}
            );
            Button button(
                200, 30, HorizontalButtonTextureAlign::CENTER,
                VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                [this]() { m_network_client.start_trade(); }, []() {},
                {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
            );
            window->add_button(
                "trade", button,
                {window->width() - 205, window->height() - 35 * 7}, true, true
            );
        }
    }  // TRADE

    {  // TAKE TOKEN
        auto *window = m_window.get_window("game");
        window->remove_button("take_token");
        if (m_network_client.m_character !=
            ::runebound::character::StandardCharacter::NONE) {
            const auto *me = m_network_client.get_yourself_character();
            const auto &pos = me->get_position();
            const auto &cell =
                m_network_client.get_game_client().m_map.m_map[pos.x][pos.y];

            if (cell.get_token() != ::runebound::AdventureType::NOTHING) {
                Texture texture;
                texture.load_text_from_string(
                    m_graphic_renderer, m_fonts["FreeMono30"], "Take token",
                    {0x00, 0x00, 0x00, 0xFF}
                );
                Button button(
                    200, 30, HorizontalButtonTextureAlign::CENTER,
                    VerticalButtonTextureAlign::CENTER, 0, 0, texture,
                    [this]() { m_network_client.take_token(); }, []() {},
                    {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
                );
                window->add_button(
                    "take_token", button,
                    {window->width() - 205, window->height() - 35 * 6}, true,
                    true
                );
            }
        } else {
            std::cout << "[[graphics log]] :: take token no self" << std::endl;
        }
    }  // TAKE TOKEN

    {  // UPDATE CHARACTERS
        SDL_Texture *tex = nullptr;
        auto *window = m_window.get_window("game")->get_window("chars");
        window->remove_all_textures();
        int counter = 0;
        RectangleShape rect;
        for (const auto &character :
             m_network_client.get_game_client().m_characters) {
            if (m_network_client.m_character !=
                    ::runebound::character::StandardCharacter::NONE &&
                character.get_name() ==
                    m_network_client.get_yourself_character()->get_name()) {
                continue;
            }
            const auto name = character.get_name();
            const auto gold = std::to_string(character.get_gold());
            const auto health = std::to_string(character.get_health());

            img_render(
                window, CHARACTER_NAMES_WITH_DASH.at(name) + "40",
                name + "char",
                {window->width() - 40, (1 + counter) * (20 * 3 + 5) - 45},
                {40, 40}
            );
            text_render(window, name, name, {0, counter * (20 * 3 + 5)}, 20);

            img_render(
                window, "coin20", name + "coin",
                {0, 20 + counter * (20 * 3 + 5)}, {20, 20}
            );
            text_render(
                window, gold, name + gold, {25, counter * (20 * 3 + 5) + 20}, 20
            );

            img_render(
                window, "heart20", name + "heart",
                {0, 40 + counter * (20 * 3 + 5)}, {20, 20}
            );
            text_render(
                window, health, name + health,
                {25, counter * (20 * 3 + 5) + 40}, 20
            );

            img_render(
                window, "body", name + "body",
                {60, 20 + counter * (20 * 3 + 5)}, {20, 20}
            );
            auto body = std::to_string(
                character.get_characteristic(::runebound::Characteristic::BODY)
            );
            text_render(
                window, body, name + "body_num",
                {85, 20 + counter * (20 * 3 + 5)}, 20
            );

            img_render(
                window, "intelligence", name + "intelligence",
                {60, 40 + counter * (20 * 3 + 5)}, {20, 20}
            );
            auto intelligence = std::to_string(character.get_characteristic(
                ::runebound::Characteristic::INTELLIGENCE
            ));
            text_render(
                window, intelligence, name + "intelligence_num",
                {85, 40 + counter * (20 * 3 + 5)}, 20
            );

            img_render(
                window, "spirit", name + "spirit",
                {120, 20 + counter * (20 * 3 + 5)}, {20, 20}
            );
            auto spirit = std::to_string(character.get_characteristic(
                ::runebound::Characteristic::SPIRIT
            ));
            text_render(
                window, body, name + "spirit_num",
                {145, 20 + counter * (20 * 3 + 5)}, 20
            );

            img_render(
                window, "knowledge", name + "knowledge",
                {120, 40 + counter * (20 * 3 + 5)}, {20, 20}
            );
            auto knowledge = std::to_string(character.get_knowledge_token());
            text_render(
                window, knowledge, name + "knowledge_num",
                {145, 40 + counter * (20 * 3 + 5)}, 20
            );

            {  // BORDER
                rect = RectangleShape(0, 0, 2 * 10 * 30 * 3 / 5, 60);
                tex = SDL_CreateTexture(
                    m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_TARGET, 2 * 10 * 30 * 3 / 5 + 1, 60 + 1
                );
                SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
                rect.render_to_texture(
                    m_graphic_renderer, tex, {0xFF, 0xFF, 0xFF, 0xFF},
                    {0x00, 0xFF, 0x00, 0xFF}
                );
                Texture texture(tex);
                window->remove_texture("_" + name + "border");
                window->add_texture(
                    " " + name + "border", texture, {0, counter * (20 * 3 + 5)},
                    true
                );
            }  // BORDER
            ++counter;
        }
    }  // UPDATE CHARACTERS

    {  // UPDATE PLAYER
        if (m_network_client.m_character !=
            character::StandardCharacter::NONE) {
            SDL_Texture *tex = nullptr;
            auto *win = m_window.get_window("game");
            auto *window = win->get_window("player");
            window->remove_all_textures();
            RectangleShape rect;
            const auto *character = m_network_client.get_yourself_character();
            const auto name = character->get_name();
            const auto gold = std::to_string(character->get_gold());
            const auto health = std::to_string(character->get_health());

            text_render(window, name, name, {0, 0}, 20);
            img_render(
                window, CHARACTER_NAMES_WITH_DASH.at(name) + "40",
                name + "char", {window->width() - 40, 0}, {40, 40}
            );

            img_render(window, "coin20", name + "coin", {0, 20}, {20, 20});
            text_render(window, gold, name + gold, {25, 20}, 20);

            img_render(window, "heart20", name + "heart", {0, 40}, {20, 20});
            text_render(window, health, name + health, {25, 40}, 20);

            img_render(window, "body", name + "body", {60, 20}, {20, 20});
            auto body = std::to_string(
                character->get_characteristic(::runebound::Characteristic::BODY)
            );
            text_render(window, body, name + "body_num", {85, 20}, 20);

            img_render(
                window, "intelligence", name + "intelligence", {60, 40},
                {20, 20}
            );
            auto intelligence = std::to_string(character->get_characteristic(
                ::runebound::Characteristic::INTELLIGENCE
            ));
            text_render(
                window, intelligence, name + "intelligence_num", {85, 40}, 20
            );

            img_render(window, "spirit", name + "spirit", {120, 20}, {20, 20});
            auto spirit = std::to_string(character->get_characteristic(
                ::runebound::Characteristic::SPIRIT
            ));
            text_render(window, body, name + "spirit_num", {145, 20}, 20);

            img_render(
                window, "knowledge", name + "knowledge", {120, 40}, {20, 20}
            );
            auto knowledge = std::to_string(character->get_knowledge_token());
            text_render(
                window, knowledge, name + "knowledge_num", {145, 40}, 20
            );

            {  // BORDER
                rect = RectangleShape(0, 0, 2 * 10 * 30 * 3 / 5, 60);
                tex = SDL_CreateTexture(
                    m_graphic_renderer, SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_TARGET, 2 * 10 * 30 * 3 / 5 + 1, 60 + 1
                );
                SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
                rect.render_to_texture(
                    m_graphic_renderer, tex, {0xFF, 0xFF, 0xFF, 0xFF},
                    {0x00, 0xFF, 0x00, 0xFF}
                );
                Texture texture(tex);
                window->remove_texture("_" + name + "border");
                window->add_texture(
                    " " + name + "border", texture, {0, 0}, true
                );
            }  // BORDER
        }
    }  // UPDATE PLAYER

    {  // SHOW FIGHT WINDOW
        auto *win = m_window.get_window("game");
        if (m_network_client.get_game_client().is_fight) {
            if (win->get_active_window_name() != "fight") {
                win->set_active_window("fight");
                win->set_visibility_window("fight", true);
                win->set_updatability_window("fight", true);
                win->set_all_updatability_button(false);
            }
            update_fight_window();
        } else if (win->get_active_window_name() == "fight") {
            win->reset_active_window();
            win->set_visibility_window("fight", false);
            win->set_updatability_window("fight", false);
            win->set_all_updatability_button(true);
        }
    }  // SHOW FIGHT WINDOW

    {  // SHOW SHOP WINDOW
        auto *win = m_window.get_window("game");
        if (m_network_client.get_yourself_character()->check_in_trade()) {
            if (win->get_active_window_name() != "shop") {
                win->set_active_window("shop");
                win->set_visibility_window("shop", true);
                win->set_updatability_window("shop", true);
                win->set_all_updatability_button(false);
            }
            update_shop_window();
        } else if (win->get_active_window_name() == "shop") {
            win->reset_active_window();
            win->set_visibility_window("shop", false);
            win->set_updatability_window("shop", false);
            win->set_all_updatability_button(true);
        }
    }  // SHOW SHOP WINDOW

    {  // INVENTORY
        auto *window = m_window.get_window("game");
        window->remove_button("inventory");
        Texture texture;
        texture.load_text_from_string(
            m_graphic_renderer, m_fonts["FreeMono30"], "Inventory",
            {0x00, 0x00, 0x00, 0xFF}
        );
        Button button(
            200, 30, HorizontalButtonTextureAlign::CENTER,
            VerticalButtonTextureAlign::CENTER, 0, 0, texture,
            [this]() {
                m_window.get_window("game")->set_active_window("inventory");
                m_window.get_window("game")->set_visibility_window(
                    "inventory", true
                );
                m_window.get_window("game")->set_updatability_window(
                    "inventory", true
                );
                m_window.get_window("game")->set_all_updatability_button(false);
            },
            []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
        );
        window->add_button(
            "inventory", button,
            {window->width() - 2 * 205, window->height() - 35 * 6}, true, true
        );
    }  // INVENTORY

    {  // INVENTORY UPDATE
        auto *win = m_window.get_window("game");
        if (win->get_active_window_name() == "inventory") {
            update_inventory_window();
        }
    }  // INVENTORY UPDATE

    {  // GAME OVER
        if (m_network_client.get_game_client().m_game_over) {
            m_window.get_window("game")->set_active_window("end");
            m_window.get_window("game")->set_visibility_window("end", true);
            m_window.get_window("game")->set_updatability_window("end", true);

            auto *win = m_window.get_window("game")->get_window("end");
            win->remove_all_textures();
            win->remove_all_buttons();
            std::string name;
            switch (m_network_client.get_game_client().m_winner) {
                case ::runebound::character::StandardCharacter::NONE:
                    name = "NONE???";
                    break;
                case character::StandardCharacter::LISSA:
                    name = "Lissa win!";
                    break;
                case character::StandardCharacter::CORBIN:
                    name = "Corbin win!";
                    break;
                case character::StandardCharacter::ELDER_MOK:
                    name = "Elder Mok win!";
                    break;
                case character::StandardCharacter::LAUREL_FROM_BLOODWOOD:
                    name = "Laurel from Bloodwood win!";
                    break;
                case character::StandardCharacter::LORD_HAWTHORNE:
                    name = "Lord Hawthorne win!";
                    break;
                case character::StandardCharacter::MASTER_THORN:
                    name = "Master Thorn win!";
                    break;
            }
            Texture tex;
            tex.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono50"], name, {0, 0, 0, 0}
            );
            win->add_texture(
                "winner", tex,
                {(win->width() - tex.width()) / 2,
                 (win->height() - tex.height()) / 2},
                true
            );
            tex.load_text_from_string(
                m_graphic_renderer, m_fonts["FreeMono30"], "Exit", {0, 0, 0, 0}
            );
            Button button(
                200, 30, HorizontalButtonTextureAlign::CENTER,
                VerticalButtonTextureAlign::CENTER, 0, 0, tex,
                [this]() {
                    m_network_client.exit_game();
                    m_window.get_window("game")->reset_active_window();
                    m_window.get_window("game")->set_visibility_window(
                        "end", false
                    );
                    m_window.get_window("game")->set_updatability_window(
                        "end", false
                    );
                    m_window.get_window("game")->set_all_updatability_button(
                        true
                    );
                    m_window.set_updatability_window("game", false);
                    m_window.set_visibility_window("game", false);
                    m_window.set_active_window("main_menu");
                    m_window.set_updatability_window("main_menu", true);
                    m_window.set_visibility_window("main_menu", true);
                },
                []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
            );
            win->add_button(
                "exit", button,
                {win->width() - button.width() - 5,
                 win->height() - button.height() - 5},
                true, true
            );
        }
    }  // GAME OVER
}
}  // namespace runebound::graphics