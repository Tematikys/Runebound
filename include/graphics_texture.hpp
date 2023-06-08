#ifndef RUNEBOUND_GRAPHICS_TEXTURE_HPP_
#define RUNEBOUND_GRAPHICS_TEXTURE_HPP_

#include <graphics_config.hpp>
#include <iostream>
#include <string>

namespace runebound::graphics {
class Texture {
private:
    SDL_Texture *m_texture{nullptr};
    int m_width{0};
    int m_height{0};

public:
    Texture();

    explicit Texture(SDL_Texture *&texture);

    Texture(Texture &&other) noexcept;
    Texture(const Texture &other) = delete;

    Texture &operator=(Texture &&other) noexcept;

    Texture &operator=(const Texture &other) = delete;

    ~Texture() {
        free();
    };

    void free();

    void render(
        SDL_Renderer *renderer,
        int x,
        int y,
        SDL_Rect *clip = nullptr,
        double angle = 0.0,
        SDL_Point *center = nullptr,
        SDL_RendererFlip flip = SDL_FLIP_NONE
    ) const;

    void render_to_texture(
        SDL_Renderer *renderer,
        int x,
        int y,
        SDL_Texture *texture
    );

    bool
    load_image_from_file(SDL_Renderer *renderer, const std::string &path);

    bool load_text_from_string(
        SDL_Renderer *renderer,
        TTF_Font *font,
        const std::string &text,
        SDL_Color color
    );

    [[nodiscard]] int width() const {
        return m_width;
    };

    [[nodiscard]] int height() const {
        return m_height;
    };
};
}  // namespace runebound::graphics
#endif  // RUNEBOUND_GRAPHICS_TEXTURE_HPP_
