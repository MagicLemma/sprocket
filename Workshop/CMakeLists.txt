cmake_minimum_required(VERSION 3.13)

project(Workshop)

set(CMAKE_CXX_STANDARD 17)

add_executable(Workshop
               Workshop.m.cpp
               WorldLayer.cpp

               EscapeMenu.cpp

               EditorUI.cpp)

target_link_libraries(Workshop PRIVATE Sprocket)
target_include_directories(Workshop PUBLIC .)