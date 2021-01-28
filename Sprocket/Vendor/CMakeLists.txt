cmake_minimum_required(VERSION 3.13)

project(SprocketVendor)
add_subdirectory(reactphysics3d)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}")

add_library(SprocketVendor STATIC

            imgui/imgui.cpp
            imgui/imgui_demo.cpp
            imgui/imgui_draw.cpp
            imgui/imgui_tables.cpp
            imgui/imgui_widgets.cpp

            ImGuizmo/ImGuizmo.cpp)

target_link_libraries(SprocketVendor PUBLIC
                      reactphysics3d)

target_include_directories(SprocketVendor PUBLIC
                           ./imgui
                           ./ImGuizmo
                           ./cpp-itertools)