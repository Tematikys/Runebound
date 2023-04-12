#ifndef RUNEBOUND_CLIENT_HPP_
#define RUNEBOUND_CLIENT_HPP_

#include <SDL2/SDL.h>
#include <graphics.hpp>
#include <graphics_board.hpp>
#include <graphics_button.hpp>
#include <graphics_config.hpp>
#include <graphics_main_menu.hpp>
#include <map>
#include <map_client.hpp>
#include <network_client.hpp>
#include <string>
#include <vector>

namespace runebound::client {
// client class, is called in main function, contains everything that is in use
class Client {
private:
    // network
    ::boost::asio::io_context m_io_context;
    ::boost::asio::executor_work_guard<::boost::asio::io_context::executor_type>
        m_work_guard = ::boost::asio::make_work_guard(m_io_context);
    ::runebound::network::Client m_network_client =
        ::runebound::network::Client(m_io_context, "127.0.0.1", 4444, "client");

    // map and board
    ::runebound::map::MapClient m_map;
    ::runebound::graphics::Board m_board;

    // graphics
    SDL_Window *m_window = nullptr;
    SDL_Renderer *m_renderer = nullptr;
    ::std::map<::std::string, TTF_Font *> m_fonts;
    ::std::vector<::runebound::graphics::Texture> m_textures;

    // buttons
    ::std::vector<::runebound::graphics::RectButton> m_rect_buttons;

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

    void handle_events();

    void render();

    void update();

    void tick();

    void exit();

    [[nodiscard]] bool running() const {
        return m_is_running;
    };
};
}  // namespace runebound::client
#endif  // RUNEBOUND_CLIENT_HPP_