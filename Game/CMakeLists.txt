cmake_minimum_required(VERSION 3.13)

project(Game)

set(CMAKE_CXX_STANDARD 17)

find_package(SFML CONFIG REQUIRED system audio)
include_directories(${SFML_INCLUDE_DIR})

add_executable(Game
               Game.m.cpp
               WorldLayer.cpp

               EscapeMenu.cpp

               EditorUI.cpp
               
               GameGrid.cpp
               CircadianCycle.cpp)

target_link_libraries(Game PRIVATE
                      Sprocket
                      sfml-system
                      sfml-audio)

target_include_directories(Game PUBLIC .)