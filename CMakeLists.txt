cmake_minimum_required(VERSION 3.10)

project(Runebound CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(NLOHMANN_INCLUDE_DIR "tpl/")
set(SDL2_INCLUDE_DIR "tpl/SDL2/include/")
set(SDL2_LIB_DIR "tpl/SDL2/lib/")

#set(SQLITE_INCLUDE_DIR "tpl/sqlite/")

include_directories("include/")
include_directories(${NLOHMANN_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIR})

#include_directories(${SQLITE_INCLUDE_DIR})

link_directories(${SDL2_LIB_DIR})

# ===== GRAPHICS CLIENT ===== #
add_executable(graphics_client
        src/card_adventure.cpp
        src/card_fight.cpp
        src/card_meeting.cpp
        src/card_research.cpp
        src/character.cpp
        src/character_client.cpp
        src/dice.cpp
        src/fight.cpp
        src/fight_two_player.cpp
        src/game.cpp
        src/game_client.cpp
        src/fight_client.cpp
        src/graphics.cpp
        src/graphics_board.cpp
        src/graphics_button.cpp
        src/graphics_character_list_window.cpp
        src/graphics_client.cpp
        src/graphics_config.cpp
        src/graphics_game_window.cpp
        src/graphics_inventory_window.cpp
        src/graphics_main.cpp
        src/graphics_main_menu_window.cpp
        src/graphics_fight_window.cpp
        src/graphics_point.cpp
        src/graphics_segment.cpp
        src/graphics_settings.cpp
        src/graphics_shapes.cpp
        src/graphics_shop_window.cpp
        src/graphics_texture.cpp
        src/graphics_window.cpp
        src/map.cpp
        src/map_cell.cpp
        src/map_client.cpp
        src/product.cpp
        tpl/SDL2/src/SDL2_framerate.cpp
        tpl/SDL2/src/SDL2_gfxPrimitives.cpp
        tpl/SDL2/src/SDL2_imageFilter.cpp
        tpl/SDL2/src/SDL2_rotozoom.cpp)
target_link_libraries(graphics_client -lmingw32 -mwindows -mconsole -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf)
target_link_libraries(graphics_client ${Boost_LIBRARIES} ws2_32 wsock32)
# ===== GRAPHICS CLIENT ===== #

# ===== NETWORK SERVER ===== #
add_executable(network_server
        src/card_adventure.cpp
        src/card_fight.cpp
        src/card_research.cpp
        src/character.cpp
        src/character_client.cpp
        src/dice.cpp
        src/fight.cpp
        src/fight_two_player.cpp
        src/game.cpp
        src/game_client.cpp
        src/fight_client.cpp
        src/card_meeting.cpp
        generators/generator_cards_fight.cpp
        generators/generator_characters.cpp
        src/map.cpp
        src/map_cell.cpp
        src/map_client.cpp
        src/network_server.cpp
        src/product.cpp
        )
target_link_libraries(network_server ${Boost_LIBRARIES} ws2_32 wsock32)
# ===== NETWORK SERVER ===== #

# ===== NETWORK CLIENT ===== #
add_executable(network_client
        src/card_adventure.cpp
        src/card_fight.cpp
        src/card_research.cpp
        src/card_meeting.cpp
        src/character.cpp
        src/fight_client.cpp
        src/character_client.cpp
        src/dice.cpp
        src/fight.cpp
        src/game.cpp
        src/game_client.cpp
        generators/generator_cards_fight.cpp
        generators/generator_characters.cpp
        src/map.cpp
        src/map_cell.cpp
        src/map_client.cpp
        src/product.cpp
        src/fight_two_player.cpp
        )
target_link_libraries(network_client ${Boost_LIBRARIES} ws2_32 wsock32)
# ===== NETWORK CLIENT ===== #

# ===== LOGICS ===== #
add_executable(${PROJECT_NAME}
        src/card_adventure.cpp
        src/card_research.cpp
        src/character.cpp
        src/dice.cpp
        src/game.cpp
        tpl/doctest/doctest_main.cpp
        src/map.cpp
        src/map_cell.cpp
        src/game_client.cpp
        src/fight_client.cpp
        src/character_client.cpp
        src/map_client.cpp
        src/fight.cpp
        src/card_fight.cpp
        src/card_meeting.cpp
        src/product.cpp
        src/fight_two_player.cpp
        #tests/test_fight.cpp
        #tests/test_fight_two_player.cpp
        tests/test_game.cpp
        generators/generator_cards_fight.cpp
        generators/generator_characters.cpp
        generators/generator_cards_meeting.cpp
        generators/generator_cards_research.cpp
        generators/generator_products.cpp
        generators/generator_map.cpp
        )
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ws2_32 wsock32)
#target_link_libraries(${PROJECT_NAME} PRIVATE pqxx)
# ===== LOGICS ===== #


