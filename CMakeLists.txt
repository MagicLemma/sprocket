cmake_minimum_required(VERSION 3.13)
project(logging-yeah)

find_package(spdlog CONFIG REQUIRED)

add_subdirectory(Sprocket)
add_subdirectory(Workshop)
