#ifndef RUNEBOUND_CLIENT_HPP_
#define RUNEBOUND_CLIENT_HPP_

#include <SDL2/SDL.h>
#include <client.hpp>
#include <graphics.hpp>
#include <graphics_board.hpp>
#include <string>
#include <utility>
#include <vector>

namespace runebound::client {
// client class, is called in main function, contains everything that is in use
class Client {
private:
    // network variables
    ::boost::asio::io_context m_io_context;
    ::boost::asio::ip::tcp::socket m_socket;
    ::std::thread m_thread;

    // board
    ::runebound::graphics::Board m_board;

    ::Client m_network_client;

    // graphics variables
    SDL_Window *m_window{nullptr};
    SDL_Renderer *m_renderer{nullptr};
    TTF_Font *m_font{nullptr};
    ::std::vector<::runebound::graphics::Texture> m_textures;
    ::std::vector<::runebound::graphics::Texture> m_temp_textures_to_render;

    bool m_is_running{false};
    int m_frame_time{};
    unsigned long long m_counter{};
    uint32_t m_prev_frame_time{};

    ::std::pair<int, int> m_mouse_pos;

public:
    Client(const std::string &host, int port, std::string user_name)
        : m_io_context(),
          m_socket(m_io_context),
          m_network_client(m_io_context, host, port, std::move(user_name)){};

    void init_graphics(
        const char *title,
        int x_pos,
        int y_pos,
        int width,
        int height,
        int fps
    );

    void init_board(const ::runebound::map::MapClient &map);

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