cmake_minimum_required(VERSION 3.10)

project(Runebound CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(NLOHMANN_INCLUDE_DIR "tpl/")
set(SDL2_INCLUDE_DIR "tpl/SDL2/include/")
set(SDL2_LIB_DIR "tpl/SDL2/lib/")

include_directories("include/")
include_directories(${BOOST_INCLUDE_DIR})
include_directories(${NLOHMANN_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIR})

link_directories(${SDL2_LIB_DIR})

# ===== GRAPHICS CLIENT ===== #
add_executable(graphics_client
        src/builder_map.cpp
        src/card_adventure.cpp
        src/card_research.cpp
        src/character.cpp
        src/character_client.cpp
        src/dice.cpp
        src/fight.cpp
        src/game.cpp
        src/game_client.cpp
        src/graphics.cpp
        src/graphics_board.cpp
        src/graphics_button.cpp
        src/graphics_client.cpp
        src/graphics_config.cpp
        src/graphics_main.cpp
        src/graphics_main_menu.cpp
        src/graphics_point.cpp
        src/graphics_segment.cpp
        src/graphics_shapes.cpp
        src/graphics_window.cpp
        src/map.cpp
        src/map_cell.cpp
        src/map_client.cpp
        src/tokens.cpp
        tpl/SDL2/src/SDL2_framerate.cpp
        tpl/SDL2/src/SDL2_gfxPrimitives.cpp
        tpl/SDL2/src/SDL2_imageFilter.cpp
        tpl/SDL2/src/SDL2_rotozoom.cpp)
target_link_libraries(graphics_client -lmingw32 -mwindows -mconsole -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf)
target_link_libraries(graphics_client ${Boost_LIBRARIES} ws2_32 wsock32)
# ===== GRAPHICS CLIENT ===== #

# ===== NETWORK SERVER ===== #
add_executable(Server
        src/card_adventure.cpp
        src/card_research.cpp
        src/character.cpp
        src/dice.cpp
        src/game.cpp
        src/map.cpp
        src/map_cell.cpp
        src/tokens.cpp
        src/builder_map.cpp
        src/game_client.cpp
        src/character_client.cpp
        src/map_client.cpp
        src/network_server.cpp
        src/fight.cpp
        src/generator_characters.cpp
        src/card_fight.cpp
        src/generator_cards_fight.cpp
        )

        
        src/generator_characters.cpp)
target_link_libraries(Server ${Boost_LIBRARIES})
target_link_libraries(Server ws2_32)
target_link_libraries(Server wsock32)
# ===== NETWORK SERVER ===== #

# ===== NETWORK CLIENT ===== #
add_executable(Client
        src/card_adventure.cpp
        src/card_research.cpp
        src/character.cpp
        src/dice.cpp
        src/game.cpp
        src/map.cpp
        src/map_cell.cpp
        src/tokens.cpp
        src/builder_map.cpp
        src/game_client.cpp
        src/character_client.cpp
        src/map_client.cpp
        src/network_client.cpp
        src/fight.cpp
        src/generator_characters.cpp)
target_link_libraries(Client ${Boost_LIBRARIES})
target_link_libraries(Client ws2_32)
target_link_libraries(Client wsock32)
# ===== NETWORK CLIENT ===== #

# ===== LOGICS? ===== #
#add_executable(${PROJECT_NAME}
#            src/card_adventure.cpp
#            src/card_research.cpp
#            src/character.cpp
#            src/dice.cpp
#            src/game.cpp
#             src/graphics.cpp
#            tpl/doctest/doctest_main.cpp
#            src/map.cpp
#            src/map_cell.cpp
#            src/tokens.cpp
#            src/builder_map.cpp
#            src/game_client.cpp
#            src/character_client.cpp
#            src/map_client.cpp
#            #src/test_fight.cpp
#            src/fight.cpp
#            #src/main.cpp
#            src/generator_characters.cpp
#            src/test_game.cpp)
#target_link_libraries(${PROJECT_NAME} PRIVATE pqxx)
