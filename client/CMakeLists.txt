cmake_minimum_required(VERSION 3.21)

project(Runebound CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(BOOST_INCLUDE_DIR "../tpl/boost/include/")
set(BOOST_LIB_DIR "../tpl/boost/lib/")
set(NLOHMANN_INCLUDE_DIR "../tpl/")
set(SDL2_INCLUDE_DIR "../tpl/SDL2/include/")
set(SDL2_LIB_DIR "../tpl/SDL2/lib/")

include_directories("../include/")
include_directories(${BOOST_INCLUDE_DIR})
include_directories(${NLOHMANN_INCLUDE_DIR})
include_directories(${SDL2_INCLUDE_DIR})

link_directories(${BOOST_LIB_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(client
        main.cpp
        client.cpp
        ../src/builder_map.cpp
        ../src/card_adventure.cpp
        ../src/card_research.cpp
        ../src/character.cpp
        ../src/dice.cpp
        ../src/game.cpp
        ../src/graphics.cpp
        ../src/graphics_board.cpp
        ../src/graphics_button.cpp
        ../src/graphics_config.cpp
        ../src/graphics_main_menu.cpp
        ../src/graphics_point.cpp
        ../src/graphics_segment.cpp
        ../src/graphics_shapes.cpp
        ../src/map.cpp
        ../src/map_cell.cpp
        ../src/map_client.cpp
        ../src/tokens.cpp
        ../tpl/SDL2/src/SDL2_framerate.cpp
        ../tpl/SDL2/src/SDL2_gfxPrimitives.cpp
        ../tpl/SDL2/src/SDL2_imageFilter.cpp
        ../tpl/SDL2/src/SDL2_rotozoom.cpp
        )

target_link_libraries(client -lmingw32 -mwindows -mconsole -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf)
target_link_libraries(client ${Boost_LIBRARIES} ws2_32 wsock32)