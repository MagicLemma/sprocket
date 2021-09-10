cmake_minimum_required(VERSION 3.13)

project(Sprocket)

add_subdirectory(Vendor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}")

include(FindLua)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(lua REQUIRED)
find_package(SFML CONFIG REQUIRED system audio)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(asio CONFIG REQUIRED)

find_path(STB_INCLUDE_DIR NAMES stb_image.h)

add_library(Sprocket STATIC
            Core/Window.cpp

            Audio/Listener.cpp
            Audio/Music.cpp
            Audio/Sound.cpp

            UI/Font/FontAtlas.cpp
            UI/Font/Font.cpp
            
            UI/UIEngine.cpp
            UI/SimpleUI.cpp
            UI/SinglePanelUI.cpp
            UI/DevUI.cpp
            UI/ImGuiXtra.cpp

            Scene/camera.cpp
            Scene/scene.cpp
            Scene/loader.cpp

            Scene/Systems/particle_system.cpp
            Scene/Systems/physics_system.cpp
            Scene/Systems/game_grid.cpp
            Scene/Systems/basic_systems.cpp
            Scene/Systems/input_system.cpp

            Graphics/Animation.cpp
            Graphics/buffer_element_types.cpp
            Graphics/buffer.cpp
            Graphics/CubeMap.cpp
            Graphics/frame_buffer.cpp
            Graphics/material.cpp
            Graphics/mesh.cpp
            Graphics/open_gl.cpp
            Graphics/post_processor.cpp
            Graphics/render_context.cpp
            Graphics/shader.cpp
            Graphics/ShadowMap.cpp
            Graphics/Texture.cpp
            Graphics/Viewport.cpp

            Graphics/Rendering/ColliderRenderer.cpp
            Graphics/Rendering/Scene3DRenderer.cpp
            Graphics/Rendering/SkyboxRenderer.cpp
            
            Utility/Log.cpp
            Utility/Colour.cpp
            Utility/FileBrowser.cpp
            Utility/InputProxy.cpp
            Utility/Random.cpp
            Utility/Maths.cpp
            Utility/Stopwatch.cpp
            Utility/Yaml.cpp

            Scripting/LuaScript.cpp
            Scripting/LuaLibrary.cpp
            Scripting/LuaMaths.cpp
            Scripting/LuaConverter.cpp)

target_link_libraries(Sprocket PUBLIC
                      SprocketVendor
                      glfw
                      glad::glad
                      glm
                      fmt::fmt
                      freetype
                      assimp::assimp
                      sfml-system
                      sfml-audio
                      yaml-cpp
                      ${LUA_LIBRARIES})

target_link_libraries(Sprocket PRIVATE asio asio::asio)

target_include_directories(Sprocket PUBLIC
                           ..
                           ./Vendor/apecs
                           ${STB_INCLUDE_DIR}
                           ${LUA_INCLUDE_DIR}
                           ${SFML_INCLUDE_DIR})