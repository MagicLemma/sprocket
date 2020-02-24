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
               
               3D/Camera.cpp
               3D/Entity.cpp
               3D/Light.cpp
               3D/Loader.cpp
               3D/Model.cpp
               3D/ObjParser.cpp
               3D/Renderer.cpp
               3D/Shader.cpp
               3D/Texture.cpp
               3D/Vertex.cpp

               Core/Layer.cpp
               Core/LayerStack.cpp
               Core/Scene.cpp
               Core/SceneData.cpp
               Core/Window.cpp

               Events/Event.cpp
               Events/WindowEvent.cpp
               Events/KeyboardEvent.cpp
               Events/MouseEvent.cpp
               
               Utility/Keyboard.cpp
               Utility/Log.cpp
               Utility/Maths.cpp
               Utility/Mouse.cpp)

target_link_libraries(Sprocket PRIVATE
                      spdlog::spdlog
                      spdlog::spdlog_header_only
                      glfw
                      glad::glad
                      glm)

target_include_directories(Sprocket PUBLIC
                           .
                           ./2D
                           ./3D
                           ./Core
                           ./Events
                           ./Utility
                           ${STB_INCLUDE_DIR})