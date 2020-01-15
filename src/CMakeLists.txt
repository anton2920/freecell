cmake_minimum_required(VERSION 3.15)
project(src)

set(CMAKE_CXX_STANDARD 20)

set(GAME libs/game.cpp libs/game.hpp)
set(CARD libs/card.cpp libs/card.hpp)
set(STATS libs/stats.cpp libs/stats.hpp)

add_executable(src main.cpp ${GAME} ${CARD} ${STATS})

target_link_libraries(src sfml-system sfml-window sfml-graphics)