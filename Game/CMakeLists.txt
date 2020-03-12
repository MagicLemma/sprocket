cmake_minimum_required(VERSION 3.13)

project(Game)

set(CMAKE_CXX_STANDARD 17)

find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_path(STB_INCLUDE_DIR NAMES stb_image.h)

add_executable(Game
               Game.m.cpp
               WorldLayer.cpp
               UILayer.cpp
               WorldSceneInfo.cpp)

target_link_libraries(Game PRIVATE
                      Sprocket
                      spdlog::spdlog
                      spdlog::spdlog_header_only
                      glfw
                      glad::glad
                      glm)

target_include_directories(Game PUBLIC
                           .
                           ${STB_INCLUDE_DIR})