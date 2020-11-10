cmake_minimum_required(VERSION 3.13)

project(Runtime)

set(CMAKE_CXX_STANDARD 17)

add_executable(Runtime
               Runtime.m.cpp
               Camera.cpp
               EditorLayer.cpp
               
               Panels/Inspector.cpp)

target_link_libraries(Runtime PRIVATE Sprocket)
target_include_directories(Runtime PUBLIC
                           .
                           ./Panels)