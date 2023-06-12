#include <graphics_client.hpp>
#include <iostream>

namespace runebound::graphics {
namespace {
SDL_Color readCol(std::string &ignore, std::ifstream &settings) {
    int r, g, b, a;
    settings >> ignore >> r >> g >> b >> a;
    return {
        static_cast<Uint8>(r), static_cast<Uint8>(g), static_cast<Uint8>(b),
        static_cast<Uint8>(a)};
}
}  // namespace

void Client::load_settings() {
    std::ifstream settings("settings.txt");
    if (settings.is_open()) {
        std::string ignore;
        settings >> ignore >> SHOW_CLIENT_DEBUG_INFO;
        settings >> ignore >> SHOW_TEXTURE_DEBUG_INFO;
        settings >> ignore >> SHOW_SHOP_DEBUG_INFO;
        settings >> ignore >> HEXAGON_RADIUS;
        SELECTED_COLOR = readCol(ignore, settings);

        settings >> ignore;
        CELL_FILL_COLOR[::runebound::map::TypeCell::WATER] =
            readCol(ignore, settings);
        CELL_FILL_COLOR[::runebound::map::TypeCell::FOREST] =
            readCol(ignore, settings);
        CELL_FILL_COLOR[::runebound::map::TypeCell::MOUNTAINS] =
            readCol(ignore, settings);
        CELL_FILL_COLOR[::runebound::map::TypeCell::HILLS] =
            readCol(ignore, settings);
        CELL_FILL_COLOR[::runebound::map::TypeCell::PLAIN] =
            readCol(ignore, settings);
        CELL_FILL_COLOR[::runebound::map::TypeCell::TOWN] =
            readCol(ignore, settings);

        settings >> ignore;
        SPECIAL_COLOR[::runebound::map::SpecialTypeCell::SETTLEMENT] =
            readCol(ignore, settings);
        SPECIAL_COLOR[::runebound::map::SpecialTypeCell::SANCTUARY] =
            readCol(ignore, settings);
        SPECIAL_COLOR[::runebound::map::SpecialTypeCell::FORTRESS] =
            readCol(ignore, settings);

        settings >> ignore;
        ADVENTURE_COLOR[::runebound::AdventureType::MEETING] =
            readCol(ignore, settings);
        ADVENTURE_COLOR[::runebound::AdventureType::RESEARCH] =
            readCol(ignore, settings);
        ADVENTURE_COLOR[::runebound::AdventureType::FIGHT] =
            readCol(ignore, settings);

        settings >> ignore;
        USED_ADVENTURE_COLOR[::runebound::AdventureType::MEETING] =
            readCol(ignore, settings);
        USED_ADVENTURE_COLOR[::runebound::AdventureType::RESEARCH] =
            readCol(ignore, settings);
        USED_ADVENTURE_COLOR[::runebound::AdventureType::FIGHT] =
            readCol(ignore, settings);

        settings >> ignore >> WINDOW_TITLE;
        settings >> ignore >> WINDOW_X_OFFSET;
        settings >> ignore >> WINDOW_Y_OFFSET;
        settings >> ignore >> WINDOW_WIDTH;
        settings >> ignore >> WINDOW_HEIGHT;
        settings >> ignore >> WINDOW_FPS;

        settings >> ignore;
        DICE_COLOR[::runebound::dice::HandDice::PLAIN] = {
            readCol(ignore, settings), readCol(ignore, settings)};
        DICE_COLOR[::runebound::dice::HandDice::PLAIN_FOREST] = {
            readCol(ignore, settings), readCol(ignore, settings)};
        DICE_COLOR[::runebound::dice::HandDice::FOREST_HILLS] = {
            readCol(ignore, settings), readCol(ignore, settings)};
        DICE_COLOR[::runebound::dice::HandDice::MOUNTAINS_WATER] = {
            readCol(ignore, settings), readCol(ignore, settings)};
        DICE_COLOR[::runebound::dice::HandDice::HILLS_PLAIN] = {
            readCol(ignore, settings), readCol(ignore, settings)};
        DICE_COLOR[::runebound::dice::HandDice::JOKER] = {
            readCol(ignore, settings), readCol(ignore, settings)};
    } else {
        throw std::runtime_error("Settings can not be opened");
    }

    std::ifstream images("images.txt");
    std::string path;
    std::string name;
    while (images >> path) {
        images >> name;
        IMAGES.emplace_back(
            path.substr(1, path.size() - 2), name.substr(1, name.size() - 2)
        );
    }

    std::ifstream fonts("fonts.txt");
    while (fonts >> path) {
        fonts >> name;
        FONTS.emplace_back(
            path.substr(1, path.size() - 2), name.substr(1, name.size() - 2)
        );
    }
}
}  // namespace runebound::graphics