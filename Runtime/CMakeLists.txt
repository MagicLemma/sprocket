cmake_minimum_required(VERSION 3.13)

project(Runtime)

set(CMAKE_CXX_STANDARD 20)

add_executable(Runtime
               Main.m.cpp
               Runtime.cpp)

target_link_libraries(Runtime PRIVATE Sprocket)
target_include_directories(Runtime PUBLIC
                           ..
                           .
                           ./Panels)