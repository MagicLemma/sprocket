cmake_minimum_required(VERSION 3.13)

project(game-ecs)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}")
set(CMAKE_CXX_STANDARD 20)

add_library(game-ecs STATIC
            loader.cpp
            lua_ecs.cpp
            scene.cpp
            yaml_ecs.cpp)

target_link_libraries(game-ecs PRIVATE Sprocket)
target_include_directories(game-ecs PUBLIC .)
