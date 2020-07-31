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

find_path(STB_INCLUDE_DIR NAMES stb_image.h)

add_library(Sprocket STATIC

            Core/CoreSystems.cpp
            Core/Window.cpp
            Core/SceneManager.cpp
            Core/ModelManager.cpp
            Core/TextureManager.cpp
            Core/Layer.cpp
            Core/Scene.cpp

            Audio/Listener.cpp
            Audio/Music.cpp
            Audio/Sound.cpp

            UI/Font/FontAtlas.cpp
            UI/Font/Font.cpp
            
            UI/UIEngine.cpp
            UI/SimpleUI.cpp
            UI/SinglePanelUI.cpp
            UI/DevUI.cpp

            Entities/Entity.cpp
            Entities/EntityManager.cpp
            Entities/EntitySystem.cpp

            Entities/Systems/BasicSelector.cpp
            Entities/Systems/Selector.cpp
            Entities/Systems/PhysicsEngine.cpp
            Entities/Systems/PlayerMovement.cpp
            Entities/Systems/ScriptRunner.cpp

            Graphics/RenderContext.cpp
            Graphics/Shader.cpp

            Graphics/Primitives/BufferLayout.cpp
            Graphics/Primitives/DepthBuffer.cpp
            Graphics/Primitives/Resources.cpp
            Graphics/Primitives/Model2D.cpp
            Graphics/Primitives/Model3D.cpp
            Graphics/Primitives/Texture.cpp
            Graphics/Primitives/CubeMap.cpp
            Graphics/Primitives/FrameBuffer.cpp
            Graphics/Primitives/StreamBuffer.cpp

            Graphics/Rendering/EntityRenderer.cpp
            Graphics/Rendering/SkyboxRenderer.cpp

            Graphics/Shadows/ShadowMapRenderer.cpp
            
            Graphics/PostProcessing/Effect.cpp
            Graphics/PostProcessing/Negative.cpp
            Graphics/PostProcessing/GaussianBlur.cpp
            Graphics/PostProcessing/PostProcessor.cpp

            Objects/CameraUtils.cpp
            Objects/Light.cpp
            Objects/Skybox.cpp
            Objects/Terrain.cpp

            Events/Event.cpp
            Events/WindowEvent.cpp
            Events/KeyboardEvent.cpp
            Events/MouseEvent.cpp
            
            Utility/Log.cpp
            Utility/Colour.cpp
            Utility/KeyboardProxy.cpp
            Utility/MouseProxy.cpp
            Utility/Random.cpp
            Utility/Maths.cpp
            Utility/Stopwatch.cpp
            Utility/Tokenize.cpp

            Scripting/LuaEngine.cpp
            Scripting/LuaGlobals.cpp
            Scripting/LuaTransform.cpp
            Scripting/LuaInput.cpp
            Scripting/LuaCamera.cpp
            
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
                      ${LUA_LIBRARIES})

target_include_directories(Sprocket PUBLIC
                           .
                           ./Audio
                           ./Core
                           ./Entities
                           ./Entities/Components
                           ./Entities/Systems
                           ./Events
                           ./Graphics
                           ./Graphics/Primitives
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