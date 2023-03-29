cmake_minimum_required(VERSION 3.10)

project(Runebound CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SDL2_INCLUDE_DIR tpl/SDL2/include/)
set(SDL2_LIB_DIR tpl/SDL2/lib/)

include_directories("include/")
include_directories("tpl/")

include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

add_executable(${PROJECT_NAME}
            src/card_adventure.cpp
            src/card_research.cpp
            src/character.cpp
            src/dice.cpp
            src/game.cpp
#             src/graphics.cpp
            src/main.cpp
            src/map.cpp
            src/map_cell.cpp
            src/tokens.cpp
        )

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} -lmingw32 -lSDL2main -lSDL2 -mwindows -lSDL2_image -mconsole)

