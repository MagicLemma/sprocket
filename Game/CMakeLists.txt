cmake_minimum_required(VERSION 3.13)

project(Game)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}")

add_executable(Game
               Game.m.cpp
               WorldLayer.cpp
               EscapeMenu.cpp
        
               CircadianCycle.cpp
               PathCalculator.cpp)

target_link_libraries(Game PRIVATE Sprocket)

target_include_directories(Game PUBLIC .)