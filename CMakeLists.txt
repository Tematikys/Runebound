cmake_minimum_required(VERSION 3.10)

project(Rubebound CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(main src/field_cell.cpp src/character.cpp src/map.cpp src/game.cpp src/main.cpp)
