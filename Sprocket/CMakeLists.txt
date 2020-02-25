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
               
               Graphics/Camera.cpp
               Graphics/Entity.cpp
               Graphics/Light.cpp
               Graphics/Loader.cpp
               Graphics/Model.cpp
               Graphics/ObjParser.cpp
               Graphics/Renderer.cpp
               Graphics/Shader.cpp
               Graphics/Texture.cpp
               Graphics/Vertex.cpp

               Graphics/GUI/Loader2D.cpp
               Graphics/GUI/Model2D.cpp
               Graphics/GUI/Vertex2D.cpp

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
                           ./Core
                           ./Events
                           ./Graphics
                           ./Graphics/GUI
                           ./Utility
                           ${STB_INCLUDE_DIR})