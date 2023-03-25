#include "client.hpp"
#include <iostream>

namespace runebound::client {
void Client::init_graphics(
    const char *title,
    int x_pos,
    int y_pos,
    int width,
    int height,
    int fps
) {
    if (!::runebound::graphics::SDL_init(
            m_window, m_renderer, title, x_pos, y_pos, width, height
        )) {
        ::std::cout << "Failed to initialize!\n";
        return;
    }
    m_is_running = true;
    m_frame_time = 1000 / fps;
}

void Client::init_board(const ::runebound::map::Map &map) {
    m_board = ::runebound::graphics::Board(map);
}

void Client::handle_events() {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            m_is_running = false;
        }
    }
}

void Client::render() {
    SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(m_renderer);

    m_board.render(m_renderer);

    SDL_RenderPresent(m_renderer);
}

void Client::update() {
    ::runebound::graphics::update_mouse_pos(m_mouse_pos);

    m_board.set_selected_hexagon(0xFFFF);
    if (auto index =
            m_board.in_bounds(::runebound::graphics::Point(m_mouse_pos))) {
        m_board.set_selected_hexagon(index.value());
    }

    ++m_counter;
}

void Client::tick() {
    uint32_t cur_frame_time = SDL_GetTicks();
    if (cur_frame_time - m_prev_frame_time < m_frame_time) {
        SDL_Delay(m_frame_time - cur_frame_time + m_prev_frame_time);
    }
    m_prev_frame_time = cur_frame_time;
}

void Client::exit() {
    SDL_DestroyWindow(m_window);
    SDL_DestroyRenderer(m_renderer);
    SDL_Quit();
}

void Client::init_network(const std::string &host, const std::string &port) {
    m_socket = ::boost::asio::ip::tcp::socket(m_io_context);
    ::boost::asio::ip::tcp::resolver resolver(m_io_context);
    ::boost::asio::ip::tcp::resolver::results_type endpoints =
        resolver.resolve(host, port);
    ::boost::asio::connect(m_socket, endpoints);
}

::std::string Client::read_network() {
    ::std::string buffer(1024, ' ');
    ::std::size_t n =
        m_socket.read_some(::boost::asio::buffer(buffer, buffer.size()));
    return buffer.substr(0, n);
}

void Client::write_network(const std::string &str) {
    ::boost::asio::write(
        m_socket, ::boost::asio::buffer(str.data(), str.length())
    );
}
}  // namespace runebound::client