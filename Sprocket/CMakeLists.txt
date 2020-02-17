cmake_minimum_required(VERSION 3.13)

project(Sprocket)

find_package(spdlog CONFIG REQUIRED)
find_path(STB_INCLUDE_DIR NAMES stb_image.h)

add_library(Sprocket STATIC
            Entry.cpp
            Log.cpp
            Vendor/STB_Image.cpp)

target_link_libraries(Sprocket PUBLIC
                      spdlog::spdlog
                      spdlog::spdlog_header_only
                      glfw)

target_include_directories(Sprocket PUBLIC
                           .
                           ./Events
                           ./Vendor
                           ${STB_INCLUDE_DIR})