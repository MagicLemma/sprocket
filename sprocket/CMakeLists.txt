cmake_minimum_required(VERSION 3.13)

project(sprocket)

add_subdirectory(vendor)

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

add_library(sprocket STATIC
            core/window.cpp
            core/timer.cpp
            core/log.cpp
            core/sound.cpp

            ui/font/font_atlas.cpp
            ui/font/font.cpp
            
            ui/console.cpp
            ui/imgui_ui.cpp
            ui/ImGuiXtra.cpp
            ui/simple_ui.cpp
            ui/ui_engine.cpp
            
            graphics/animation.cpp
            graphics/buffer_element_types.cpp
            graphics/buffer.cpp
            graphics/camera.cpp
            graphics/cube_map.cpp
            graphics/frame_buffer.cpp
            graphics/material.cpp
            graphics/mesh.cpp
            graphics/open_gl.cpp
            graphics/post_processor.cpp
            graphics/render_context.cpp
            graphics/shader.cpp
            graphics/shadow_map.cpp
            graphics/texture.cpp
            graphics/viewport.cpp

            graphics/renderers/geometry_renderer.cpp
            graphics/renderers/shape_renderer.cpp
            graphics/renderers/pbr_renderer.cpp
            graphics/renderers/skybox_renderer.cpp
            
            utility/colour.cpp
            utility/file_browser.cpp
            utility/input_store.cpp
            utility/random.cpp
            utility/maths.cpp
            utility/yaml.cpp

            scripting/lua_script.cpp
            scripting/lua_maths.cpp
            scripting/lua_converter.cpp
            scripting/lua_input.cpp)

target_link_libraries(sprocket PUBLIC
                      imgui
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

target_link_libraries(sprocket PRIVATE asio asio::asio)

target_include_directories(sprocket PUBLIC
                           ..
                           ./vendor/apecs
                           ${STB_INCLUDE_DIR}
                           ${LUA_INCLUDE_DIR}
                           ${SFML_INCLUDE_DIR})