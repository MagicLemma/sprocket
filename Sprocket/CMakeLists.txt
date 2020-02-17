cmake_minimum_required(VERSION 3.13)

project(Sprocket)

find_package(spdlog CONFIG REQUIRED)

add_library(Sprocket STATIC
            Entry.cpp)

target_link_libraries(Sprocket PUBLIC spdlog::spdlog spdlog::spdlog_header_only)

target_include_directories(Sprocket PUBLIC
                           .
                           ./Events)