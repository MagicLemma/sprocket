cmake_minimum_required(VERSION 3.13)

project(kinematica)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}")
set(CMAKE_CXX_STANDARD 20)

add_executable(kinematica
               kinematica.m.cpp)

target_link_libraries(kinematica PRIVATE sprocket)
target_include_directories(kinematica PUBLIC .)