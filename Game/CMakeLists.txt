cmake_minimum_required(VERSION 3.13)

project(game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}")

add_executable(game
               game.m.cpp
               Game.cpp
        
               CircadianCycle.cpp
               PathCalculator.cpp
               
               grid_helpers.cpp)

target_link_libraries(game PRIVATE Sprocket)

target_include_directories(game PUBLIC . ..)