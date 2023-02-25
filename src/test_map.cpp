#include "../include/map.hpp"

int main() {
    using namespace runebound;
    {
    field::Map test_map;
    for (int i = 0; i < 5; i++) {      // координата y
        for (int j = 0; j < 5; j++) {  // координата x
            test_map.get_neighbours_coor(j,i);
            char c = ' ';
            for (auto [x, y] : test_map.get_neighbours_coor(j, i)) {  // тест на наличие реки рядом с клеткой
                if (test_map.field::Map::check_river(field::make_river_index(j, i, x, y))) {
                    c = '*';
                }
            }
            if (test_map.get_cell_map(j, i).get_token() !=AdventureType::NOTHING) {  // тест на приключения в клетке
                c = '^';
            }
            if (i % 2 == 0) {
                std::cout << "  " << static_cast<int>(test_map.get_cell_map(i, j).get_type_cell()) << c << " ";
            } else {
                std::cout << static_cast<int>(test_map.get_cell_map(i, j).get_type_cell()) << c << "   ";
            }
        }
        std::cout << '\n';
    }
    }


    return 0;
}