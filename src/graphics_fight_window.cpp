#include <graphics_client.hpp>

namespace runebound::graphics {
void Client::init_fight_window() {
    auto window = ::std::make_unique<Window>(Window(
        m_graphic_renderer, WINDOW_WIDTH * 3 / 4, WINDOW_HEIGHT * 3 / 4,
        {0xFF, 0xFF, 0xFF, 0xFF}
    ));
    Texture texture;
    Button button;

    // ===== CLOSE BUTTON =====
    texture.load_text_from_string(
        m_graphic_renderer, m_fonts["FreeMono30"], "Exit",
        {0x00, 0x00, 0x00, 0xFF}
    );
    button = Button(
        10 * 18, 30, HorizontalButtonTextureAlign::CENTER,
        VerticalButtonTextureAlign::CENTER, 0, 0, texture,
        [this]() {
            ::std::cout << "Clicked!" << ::std::endl;
            m_window.get_window("main_menu")->get_window("fight")->deactivate();
            m_window.get_window("main_menu")
                ->set_visibility_window("fight", false);
            m_window.get_window("main_menu")
                ->set_updatability_window("fight", false);
        },
        []() {}, {0xFF, 0xFF, 0xFF, 0xFF}, {0x00, 0x00, 0x00, 0xFF}
    );
    window->add_button(
        "exit", button,
        {WINDOW_WIDTH * 3 / 4 - 10 * 18 - 5, WINDOW_HEIGHT * 3 / 4 - 35 * 1},
        true, true
    );
    // ===== CLOSE BUTTON =====

    m_window.get_window("main_menu")
        ->add_window(
            "fight", ::std::move(window), {WINDOW_WIDTH / 8, WINDOW_HEIGHT / 8},
            true, true
        );

    //    m_window.add_window_in_window(
    //        "main_menu", "fight", ::std::move(window),
    //        {WINDOW_WIDTH / 8, WINDOW_HEIGHT / 8}, true, true
    //    );
}
}  // namespace runebound::graphics