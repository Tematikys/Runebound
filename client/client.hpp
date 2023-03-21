#ifndef RUNEBOUND_CLIENT_HPP_
#define RUNEBOUND_CLIENT_HPP_

#include <SDL2/SDL.h>
#include <graphics.hpp>
#include <graphics_board.hpp>

namespace runebound::client {
class Client {
private:
    SDL_Window *m_window{nullptr};
    SDL_Renderer *m_renderer{nullptr};
    bool m_is_running{false};
    unsigned long long m_counter{0};
    ::runebound::graphics::Board m_board;
    int m_fps{1000 / 60};
    uint32_t m_prev_frame_time{0};

public:
    void init_graphics(
        const char *title,
        int x_pos,
        int y_pos,
        int width,
        int height
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