cmake_minimum_required(VERSION 3.24)

project(Runebound CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SDL2_INCLUDE_DIR "../tpl/SDL2/include/")
set(NLOHMANN_INCLUDE_DIR "../tpl/nlohmann/")
set(SDL2_LIB_DIR "../tpl/SDL2/lib/")

include_directories("../include/")
include_directories(${SDL2_INCLUDE_DIR})
include_directories(${NLOHMANN_INCLUDE_DIR})

link_directories(${SDL2_LIB_DIR})

add_executable(client
        main.cpp
        client.cpp
        ../src/graphics.cpp
        ../src/graphics_board.cpp
        ../src/graphics_shapes.cpp
        ../src/map.cpp
        ../src/map_cell.cpp
        ../src/dice.cpp
        )

target_link_libraries(client -lmingw32 -mwindows -mconsole -lSDL2main -lSDL2)