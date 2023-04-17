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


include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})

#add_executable(Server
#
#        src/card_adventure.cpp
#        src/card_research.cpp
#        src/character.cpp
#        src/dice.cpp
#        src/game.cpp
#        #             src/graphics.cpp
#        src/map.cpp
#        src/map_cell.cpp
#        src/tokens.cpp
#        src/builder_map.cpp
#        src/game_client.cpp
#        src/character_client.cpp
#        src/map_client.cpp
#        src/network_server.cpp
#        src/fight.cpp
#        src/generator_characters.cpp
#        src/card_fight.cpp
#        src/generator_cards_fight.cpp
#        )
#add_executable(Client
#
#        src/card_adventure.cpp
#        src/card_research.cpp
#        src/character.cpp
#        src/dice.cpp
#        src/game.cpp
#        #             src/graphics.cpp
#        src/map.cpp
#        src/map_cell.cpp
#        src/tokens.cpp
#        src/builder_map.cpp
#        src/game_client.cpp
#        src/character_client.cpp
#        src/map_client.cpp
#        src/network_client.cpp
#        src/fight.cpp
#        src/generator_characters.cpp
#        src/card_fight.cpp
#        src/generator_cards_fight.cpp
#        )
#
#





add_executable(${PROJECT_NAME}
            src/card_adventure.cpp
            src/card_research.cpp
            src/character.cpp
            src/dice.cpp
            src/game.cpp
#             src/graphics.cpp
            tpl/doctest/doctest_main.cpp
            src/map.cpp
            src/map_cell.cpp
            src/tokens.cpp
            src/builder_map.cpp
            src/game_client.cpp
            src/character_client.cpp
            src/map_client.cpp
            #src/test_fight.cpp
            src/fight.cpp
            src/generator_characters.cpp
            #src/test_game.cpp
            src/card_fight.cpp
            src/generator_cards_fight.cpp
        )



#target_link_libraries(Server ${Boost_LIBRARIES})
#target_link_libraries(Server ws2_32)
#target_link_libraries(Server wsock32)
#target_link_libraries(Client ${Boost_LIBRARIES})
#target_link_libraries(Client ws2_32)
#target_link_libraries(Client wsock32)


#target_link_libraries(${PROJECT_NAME} PRIVATE pqxx)
