#ifndef RUNEBOUND_CLIENT_HPP_
#define RUNEBOUND_CLIENT_HPP_

#include <SDL2/SDL.h>
#include <graphics.hpp>
#include <graphics_board.hpp>

namespace runebound::client {
class Client {
private:
    ::runebound::graphics::Board m_board;
    SDL_Window *m_window;
    SDL_Renderer *m_renderer;
    bool m_is_running;
    int m_frame_time;
    unsigned long long m_counter;
    uint32_t m_prev_frame_time;

public:
    Client()
        : m_board(),
          m_window(nullptr),
          m_renderer(nullptr),
          m_is_running(false),
          m_frame_time(1000 / 60),
          m_counter(0),
          m_prev_frame_time(0){};

    void init_graphics(
        const char *title,
        int x_pos,
        int y_pos,
        int width,
        int height,
        int fps
    );

    void init_board(const ::runebound::map::Map &map);

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