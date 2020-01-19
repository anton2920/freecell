cmake_minimum_required(VERSION 3.15)
project(src)

set(CMAKE_CXX_STANDARD 20)

set(GAME libs/game.cpp libs/drawSystem.cpp libs/menu.cpp libs/stats.cpp libs/game.hpp)
set(CARD libs/card.cpp libs/card.hpp)
set(PLACEHOLDER libs/holders/placeholder.cpp libs/holders/placeholder.hpp)
set(SAFEHOUSE libs/holders/safehouse.cpp libs/holders/safehouse.hpp)
set(ENDGAME libs/holders/endgame.cpp libs/holders/endgame.hpp)

add_executable(src main.cpp ${GAME} ${CARD} ${PLACEHOLDER} ${SAFEHOUSE} ${ENDGAME})

target_link_libraries(src sfml-system sfml-window sfml-graphics tgui)