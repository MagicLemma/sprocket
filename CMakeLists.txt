cmake_minimum_required(VERSION 3.13)
project(logging-yeah)

find_package(spdlog CONFIG REQUIRED)

add_subdirectory(Sprocket)

add_executable(logging-yeah main.cpp)
target_link_libraries(logging-yeah PRIVATE spdlog::spdlog spdlog::spdlog_header_only Sprocket)
