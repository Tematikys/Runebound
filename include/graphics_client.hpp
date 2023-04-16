#ifndef RUNEBOUND_CLIENT_HPP_
#define RUNEBOUND_CLIENT_HPP_

#include <SDL2/SDL.h>
#include <game_client.hpp>
#include <graphics.hpp>
#include <graphics_board.hpp>
#include <graphics_button.hpp>
#include <graphics_config.hpp>
#include <map>
#include <network_client.hpp>
#include <string>
#include <vector>

namespace runebound::client {
// client class, is called in main function, contains everything that is in use
class Client {
private:
    // network
    bool m_is_joined_to_game = false;
    ::boost::asio::io_context m_io_context;
    ::boost::asio::executor_work_guard<::boost::asio::io_context::executor_type>
        m_work_guard = ::boost::asio::make_work_guard(m_io_context);
    ::runebound::network::Client m_network_client =
        ::runebound::network::Client(m_io_context, "127.0.0.1", 4444, "client");

    // game and board
    ::runebound::game::GameClient m_game;
    ::runebound::graphics::Board m_board;
    ::std::size_t m_game_list_start_index = 0;
    ::std::size_t m_game_list_show_amount = 10;
    ::std::vector<::runebound::graphics::Button> m_game_list;

    // graphics
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;
    ::std::map<::std::string, TTF_Font *> m_fonts;

    // main menu
    ::std::vector<::runebound::graphics::Texture> m_main_menu_textures;
    ::std::vector<::runebound::graphics::Button> m_main_menu_buttons;
    ::std::vector<::runebound::graphics::TextField> m_main_menu_text_fields;
    ::std::size_t m_main_menu_active_text_field;

    // time
    bool m_is_running = false;
    uint32_t m_frame_time;
    uint64_t m_counter = 0;
    uint32_t m_prev_frame_time = 0;

    // mouse
    bool m_mouse_pressed = false;
    ::runebound::graphics::Point m_mouse_pos;

public:
    void init();

    void init_graphics();

    void init_board();

    void init_main_menu();

    void load_fonts();

    void handle_events();

    void game_render();

    void main_menu_render();

    void render();

    void game_update();

    void main_menu_update();

    void update();

    void tick();

    void exit();

    [[nodiscard]] bool running() const {
        return m_is_running;
    };
};
}  // namespace runebound::client
#endif  // RUNEBOUND_CLIENT_HPP_