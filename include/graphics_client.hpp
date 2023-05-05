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

    bool m_joined_to_game{false};
    ::boost::asio::io_context m_io_context;
    ::boost::asio::executor_work_guard<::boost::asio::io_context::executor_type>
        m_work_guard = ::boost::asio::make_work_guard(m_io_context);
    ::runebound::network::Client m_network_client =
        ::runebound::network::Client(m_io_context, "127.0.0.1", 4444, "client");

    Board m_board{};
    Point m_board_pos{0, 0};
    ::std::size_t m_game_list_start_index{0};
    ::std::size_t m_game_list_show_amount{10};
    ::std::vector<Button> m_game_list{};
    ::std::vector<Point> m_game_list_pos{};

    SDL_Window *m_graphic_window{nullptr};
    SDL_Renderer *m_graphic_renderer{nullptr};
    ::std::map<::std::string, TTF_Font *> m_fonts{};
    ::std::map<::std::string, Texture> m_images{};

    ::std::vector<Texture> m_game_textures{};
    ::std::vector<Button> m_game_buttons{};
    ::std::vector<Point> m_game_button_pos{};
    ::std::vector<Button> m_character_list{};
    ::std::vector<Point> m_character_list_pos{};
    bool m_character_selected{false};

    bool m_is_running{false};
    uint32_t m_frame_time{0};
    uint64_t m_counter{0};
    uint32_t m_prev_frame_time{0};

    bool m_mouse_pressed{false};
    Point m_mouse_pos{};

public:
    Client() = default;

    void load_fonts();

    void load_images();

    void init();

    void init_graphics();

    void update_board();

    void init_game();

    void init_game_list();

    void init_main_menu();

    void handle_events();

    void game_render();

    void render();

    void game_update();

    void main_menu_update();

    void char_list_update();

    void update();

    void tick();

    void exit();

    [[nodiscard]] bool running() const {
        return m_is_running;
    };
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_CLIENT_HPP_