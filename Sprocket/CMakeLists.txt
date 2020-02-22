cmake_minimum_required(VERSION 3.13)

project(Sprocket)

set(CMAKE_CXX_STANDARD 17)

find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_path(STB_INCLUDE_DIR NAMES stb_image.h)

add_executable(Sprocket
               Sprocket.m.cpp
               Camera.cpp
               Entity.cpp
               Keyboard.cpp
               Light.cpp
               Loader.cpp
               Log.cpp
               Maths.cpp
               Model.cpp
               ObjParser.cpp
               Renderer.cpp
               Shader.cpp
               Window.cpp
               Texture.cpp
               Vertex.cpp
               Events/Event.cpp
               Events/WindowEvent.cpp
               Events/KeyboardEvent.cpp
               Events/MouseEvent.cpp)

target_link_libraries(Sprocket PRIVATE
                      spdlog::spdlog
                      spdlog::spdlog_header_only
                      glfw
                      glad::glad
                      glm)

target_include_directories(Sprocket PUBLIC
                           .
                           ./Events
                           ./Vendor
                           ${STB_INCLUDE_DIR})