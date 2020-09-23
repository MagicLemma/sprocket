cmake_minimum_required(VERSION 3.13)

project(Sprocket)
add_subdirectory(Vendor/reactphysics3d)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}")

include(FindLua)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(lua REQUIRED)
find_package(SFML CONFIG REQUIRED system audio)
find_package(EnTT CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

find_path(STB_INCLUDE_DIR NAMES stb_image.h)

add_library(Sprocket STATIC

            Core/App.cpp
            Core/Window.cpp
            Core/MaterialManager.cpp
            Core/ModelManager.cpp
            Core/TextureManager.cpp

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

            Scene/Entity.cpp
            Scene/Scene.cpp
            Scene/EntitySystem.cpp
            Scene/Loader.cpp
            Scene/Updater.cpp

            Scene/Systems/BasicSelector.cpp
            Scene/Systems/CameraSystem.cpp
            Scene/Systems/Selector.cpp
            Scene/Systems/ParticleSystem.cpp
            Scene/Systems/PhysicsEngine.cpp
            Scene/Systems/ScriptRunner.cpp
            Scene/Systems/GameGrid.cpp
            Scene/Systems/PathFollower.cpp

            Graphics/Buffer.cpp
            Graphics/BufferLayout.cpp
            Graphics/RenderContext.cpp
            Graphics/Shader.cpp
            Graphics/DepthBuffer.cpp
            Graphics/Resources.cpp
            Graphics/Material.cpp
            Graphics/Model2D.cpp
            Graphics/Model3D.cpp
            Graphics/Texture.cpp
            Graphics/CubeMap.cpp
            Graphics/FrameBuffer.cpp
            Graphics/ParticleManager.cpp
            Graphics/ShadowMap.cpp
            Graphics/StreamBuffer.cpp
            Graphics/VertexArray.cpp

            Graphics/Rendering/ColliderRenderer.cpp
            Graphics/Rendering/EntityRenderer.cpp
            Graphics/Rendering/SkyboxRenderer.cpp

            
            Graphics/PostProcessing/Effect.cpp
            Graphics/PostProcessing/Negative.cpp
            Graphics/PostProcessing/GaussianBlur.cpp
            Graphics/PostProcessing/PostProcessor.cpp

            Objects/CameraUtils.cpp
            Objects/Skybox.cpp
            Objects/Terrain.cpp

            Events/Event.cpp
            Events/WindowEvent.cpp
            Events/KeyboardEvent.cpp
            Events/MouseEvent.cpp
            
            Utility/Log.cpp
            Utility/Colour.cpp
            Utility/FileBrowser.cpp
            Utility/KeyboardProxy.cpp
            Utility/MouseProxy.cpp
            Utility/Random.cpp
            Utility/Maths.cpp
            Utility/Stopwatch.cpp
            Utility/Tokenize.cpp
            Utility/Yaml.cpp

            Scripting/LuaEngine.cpp
            Scripting/LuaEntity.cpp
            Scripting/LuaComponents.cpp
            Scripting/LuaGlobals.cpp
            Scripting/LuaTransform.cpp
            Scripting/LuaInput.cpp
            
            Vendor/ImGuizmo/ImGuizmo.cpp)

target_link_libraries(Sprocket PUBLIC
                      glfw
                      glad::glad
                      glm
                      spdlog::spdlog
                      freetype
                      reactphysics3d
                      assimp::assimp
                      imgui::imgui
                      sfml-system
                      sfml-audio
                      EnTT::EnTT
                      yaml-cpp
                      ${LUA_LIBRARIES})

target_include_directories(Sprocket PUBLIC
                           .
                           ./Audio
                           ./Core
                           ./Scene
                           ./Scene/Systems
                           ./Events
                           ./Graphics
                           ./Graphics/Rendering
                           ./Graphics/Shadows
                           ./Graphics/PostProcessing
                           ./Objects
                           ./Objects/Lenses
                           ./Utility
                           ./Scripting
                           ./UI
                           ./UI/Font
                           ./Vendor/ImGuizmo
                           ./Vendor/ftgl
                           ${STB_INCLUDE_DIR}
                           ${LUA_INCLUDE_DIR}
                           ${SFML_INCLUDE_DIR})