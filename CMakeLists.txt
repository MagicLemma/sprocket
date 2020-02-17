cmake_minimum_required(VERSION 3.13)
project(Sprocket)

find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_subdirectory(Sprocket)
add_subdirectory(Workshop)
