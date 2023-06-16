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

        const std::initializer_list<::runebound::map::TypeCell> allTypeCell = {
            ::runebound::map::TypeCell::WATER,
            ::runebound::map::TypeCell::FOREST,
            ::runebound::map::TypeCell::MOUNTAINS,
            ::runebound::map::TypeCell::HILLS,
            ::runebound::map::TypeCell::PLAIN,
            ::runebound::map::TypeCell::TOWN};

        settings >> ignore;
        for (auto key : allTypeCell) {
            CELL_FILL_COLOR[key] = readCol(ignore, settings);
        }
        const std::initializer_list<::runebound::map::SpecialTypeCell>
            allSpecialTypeCell = {
                ::runebound::map::SpecialTypeCell::SETTLEMENT,
                ::runebound::map::SpecialTypeCell::SANCTUARY,
                ::runebound::map::SpecialTypeCell::FORTRESS};

        settings >> ignore;
        for (auto key : allSpecialTypeCell) {
            SPECIAL_COLOR[key] = readCol(ignore, settings);
        }

        const std::initializer_list<::runebound::AdventureType>
            allAdventureType = {
                ::runebound::AdventureType::MEETING,
                ::runebound::AdventureType::RESEARCH,
                ::runebound::AdventureType::FIGHT};

        settings >> ignore;
        for (auto key : allAdventureType) {
            ADVENTURE_COLOR[key] = readCol(ignore, settings);
        }

        settings >> ignore;
        for (auto key : allAdventureType) {
            USED_ADVENTURE_COLOR[key] = readCol(ignore, settings);
        }

        settings >> ignore >> WINDOW_TITLE;
        settings >> ignore >> WINDOW_X_OFFSET;
        settings >> ignore >> WINDOW_Y_OFFSET;
        settings >> ignore >> WINDOW_WIDTH;
        settings >> ignore >> WINDOW_HEIGHT;
        settings >> ignore >> WINDOW_FPS;

        const std::initializer_list<::runebound::dice::HandDice> allHandDice = {
            ::runebound::dice::HandDice::JOKER,
            ::runebound::dice::HandDice::PLAIN,
            ::runebound::dice::HandDice::PLAIN_FOREST,
            ::runebound::dice::HandDice::FOREST_HILLS,
            ::runebound::dice::HandDice::MOUNTAINS_WATER,
            ::runebound::dice::HandDice::HILLS_PLAIN};

        settings >> ignore;
        for (auto key : allHandDice) {
            DICE_COLOR[key] = {
                readCol(ignore, settings), readCol(ignore, settings)};
        }
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