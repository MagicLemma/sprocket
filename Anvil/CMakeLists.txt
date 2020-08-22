cmake_minimum_required(VERSION 3.13)

project(Anvil)

set(CMAKE_CXX_STANDARD 17)

add_executable(Anvil
               Anvil.m.cpp
               WorldLayer.cpp

               EscapeMenu.cpp

               EditorUI.cpp)

target_link_libraries(Anvil PRIVATE Sprocket)
target_include_directories(Anvil PUBLIC .)