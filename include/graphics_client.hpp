#ifndef RUNEBOUND_CLIENT_HPP_
#define RUNEBOUND_CLIENT_HPP_

#include <SDL2/SDL.h>
#include <character.hpp>
#include <game_client.hpp>
#include <graphics.hpp>
#include <graphics_board.hpp>
#include <graphics_button.hpp>
#include <graphics_config.hpp>
#include <graphics_window.hpp>
#include <map.hpp>
#include <map>
#include <network_client.hpp>
#include <string>
#include <vector>

namespace runebound::graphics {
class Client {
private:
    Window m_window;
    bool m_need_to_update{true};

    ::boost::asio::io_context m_io_context;
    ::boost::asio::executor_work_guard<::boost::asio::io_context::executor_type>
        m_work_guard = ::boost::asio::make_work_guard(m_io_context);
    ::runebound::network::Client m_network_client =
        ::runebound::network::Client(m_io_context, "127.0.0.1", 4444, "client");

    Board m_board{};
    Point m_board_pos{5, 5};
    ::std::size_t m_game_list_start_index{0};
    ::std::size_t m_game_list_show_amount{10};

    SDL_Window *m_graphic_window{nullptr};
    SDL_Renderer *m_graphic_renderer{nullptr};
    ::std::map<::std::string, TTF_Font *> m_fonts{};
    ::std::map<::std::string, Texture> m_images{};

    bool m_is_running{false};
    uint32_t m_frame_time{0};
    uint64_t m_counter{0};
    uint32_t m_prev_frame_time{0};

    bool m_mouse_pressed{false};
    Point m_mouse_pos{};
    Point m_prev_mouse_pos{};

public:
    Client() = default;

    void update_board();

    void load_fonts();

    void load_images();

    void init();

    void init_graphics();

    void init_game_window();

    void init_fight_window();

    void init_character_list_window();

    void init_main_menu_window();

    void handle_events();

    void render();

    void update_game_window();

    void update_main_menu_window();

    void update_character_list_window();

    void update_fight_window();

    void update();

    void tick();

    void exit();

    [[nodiscard]] bool is_running() const {
        return m_is_running;
    };
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_CLIENT_HPP_