cmake_minimum_required(VERSION 3.13)

project(Sprocket)

find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_path(STB_INCLUDE_DIR NAMES stb_image.h)

add_library(Sprocket STATIC
            Application.cpp
            Entry.cpp
            Log.cpp
            Window.cpp
            Events/Event.cpp
            Events/WindowEvent.cpp
            Events/KeyboardEvent.cpp
            Events/MouseEvent.cpp
            Vendor/STB_Image.cpp)

target_link_libraries(Sprocket PRIVATE
                      spdlog::spdlog
                      spdlog::spdlog_header_only
                      glfw
                      glad::glad)

target_include_directories(Sprocket PUBLIC
                           .
                           ./Events
                           ./Vendor
                           ${STB_INCLUDE_DIR})