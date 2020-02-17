cmake_minimum_required(VERSION 3.13)

project(Workshop)

find_package(spdlog CONFIG REQUIRED)

add_executable(Workshop
               Workshop.cpp)

target_link_libraries(Workshop PUBLIC
                      spdlog::spdlog
                      spdlog::spdlog_header_only
                      Sprocket)

target_include_directories(Workshop PUBLIC
                           .)