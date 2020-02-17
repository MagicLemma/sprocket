cmake_minimum_required(VERSION 3.13)

project(logging-library)

find_package(spdlog CONFIG REQUIRED)

add_library(logging-library STATIC library.cpp)
target_link_libraries(logging-library PUBLIC spdlog::spdlog spdlog::spdlog_header_only)
target_include_directories(logging-library PUBLIC .)
target_compile_definitions(logging-library PRIVATE IS_LIB)