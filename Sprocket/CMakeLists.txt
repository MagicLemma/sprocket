cmake_minimum_required(VERSION 3.13)

project(Sprocket)
add_subdirectory(Vendor/reactphysics3d)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS}")

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_path(STB_INCLUDE_DIR NAMES stb_image.h)

add_library(Sprocket STATIC

            Core/CoreSystems.cpp
            Core/Window.cpp
            Core/SceneManager.cpp
            Core/ModelManager.cpp
            Core/Layer.cpp
            Core/Scene.cpp

            DevGUI/DevGUI.cpp

            Graphics/MousePicker.cpp
            Graphics/RenderContext.cpp
            Graphics/RenderOptions.cpp
            Graphics/Shader.cpp

            Graphics/Primitives/Resources.cpp
            Graphics/Primitives/Model2D.cpp
            Graphics/Primitives/Model3D.cpp
            Graphics/Primitives/Texture.cpp
            Graphics/Primitives/CubeMap.cpp
            Graphics/Primitives/FrameBuffer.cpp
            Graphics/Primitives/StreamBuffer.cpp

            Graphics/Rendering/DevGUIRenderer.cpp
            Graphics/Rendering/DisplayRenderer.cpp
            Graphics/Rendering/EntityRenderer.cpp
            Graphics/Rendering/SkyboxRenderer.cpp
            Graphics/Rendering/TerrainRenderer.cpp

            Graphics/Rendering/Constraints/AspectConstraint.cpp
            
            Graphics/PostProcessing/Effect.cpp
            Graphics/PostProcessing/Negative.cpp
            Graphics/PostProcessing/GaussianBlur.cpp
            Graphics/PostProcessing/PostProcessor.cpp

            Display/Widget.cpp
            Display/Container.cpp
            Display/Slider.cpp
            Display/Button.cpp
            Display/ColourPalette.cpp
            Display/Image.cpp
            Display/TextBox.cpp

            Display/Fonts/Character.cpp
            Display/Fonts/Font.cpp
            Display/Fonts/FontLoader.cpp
            Display/Fonts/Text.cpp

            Display/WidgetProperties/WidgetProperty.cpp
            Display/WidgetProperties/Draggable.cpp
            Display/WidgetProperties/HorizontalConstraint.cpp
            Display/WidgetProperties/VerticalConstraint.cpp

            Objects/Entities/Entity.cpp
            Objects/Entities/Components.cpp
            Objects/Entities/EntityManager.cpp

            Objects/Cameras/Camera.cpp
            Objects/Cameras/FirstPersonCamera.cpp
            Objects/Cameras/PlayerBasedCamera.cpp
            Objects/Cameras/ThirdPersonCamera.cpp

            Objects/Lenses/Lens.cpp
            Objects/Lenses/PerspectiveLens.cpp

            Objects/Light.cpp
            Objects/Material.cpp
            Objects/Quad.cpp
            Objects/Skybox.cpp
            Objects/Terrain.cpp

            Systems/EntitySystem.cpp
            Systems/PhysicsEngine.cpp
            Systems/Colliders.cpp
            Systems/PlayerMovement.cpp
            Systems/Selector.cpp

            Events/Event.cpp
            Events/WindowEvent.cpp
            Events/KeyboardEvent.cpp
            Events/MouseEvent.cpp
            
            Utility/FramerateTimer.cpp
            Utility/Log.cpp
            Utility/Maths.cpp
            Utility/Tokenize.cpp)

target_link_libraries(Sprocket PUBLIC
                      glfw
                      glad::glad
                      glm
                      spdlog::spdlog
                      freetype
                      reactphysics3d
                      assimp::assimp
                      imgui::imgui)

target_include_directories(Sprocket PUBLIC
                           .
                           ./Core
                           ./DevGUI
                           ./Display
                           ./Display/Fonts
                           ./Display/WidgetProperties
                           ./Events
                           ./Graphics
                           ./Graphics/Primitives
                           ./Graphics/Rendering
                           ./Graphics/PostProcessing
                           ./Objects
                           ./Objects/Entities
                           ./Objects/Cameras
                           ./Objects/Lenses
                           ./Systems
                           ./Utility
                           ${STB_INCLUDE_DIR})