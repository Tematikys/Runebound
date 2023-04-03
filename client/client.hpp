#ifndef RUNEBOUND_CLIENT_HPP_
#define RUNEBOUND_CLIENT_HPP_

#include <SDL2/SDL.h>
#include <boost/asio.hpp>
#include <graphics.hpp>
#include <graphics_board.hpp>
#include <string>
#include <utility>

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

    // graphics variables
    SDL_Window *m_window{nullptr};
    SDL_Renderer *m_renderer{nullptr};
    bool m_is_running{false};
    int m_frame_time{};
    unsigned long long m_counter{};
    uint32_t m_prev_frame_time{};
    ::std::pair<int, int> m_mouse_pos;

public:
    Client() : m_io_context(), m_socket(m_io_context){};

    void init_network(const ::std::string &host, const ::std::string &port);

    void start_network() {
        m_thread = ::std::thread([this]() { m_io_context.run(); });
    }

    [[nodiscard]] ::std::string read_network();

    void write_network(const ::std::string &str);

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