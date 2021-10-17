cmake_minimum_required(VERSION 3.13)

add_subdirectory(ecs)

project(game)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}")

add_executable(game
               game.m.cpp
               game.cpp
        
               day_night_cycle.cpp
               path_calculator.cpp
               
               grid_helpers.cpp
               game_grid.cpp)

target_link_libraries(game PRIVATE Sprocket game-ecs)

target_include_directories(game PUBLIC .)