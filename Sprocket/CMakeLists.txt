cmake_minimum_required(VERSION 3.13)

project(Sprocket)

set(CMAKE_CXX_STANDARD 17)

find_package(spdlog CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_path(STB_INCLUDE_DIR NAMES stb_image.h)

add_library(Sprocket STATIC

            Graphics/Loader.cpp
            Graphics/ObjParser.cpp
            Graphics/RenderOptions.cpp
            Graphics/Shader.cpp

            Graphics/Primitives/Model2D.cpp
            Graphics/Primitives/Model3D.cpp
            Graphics/Primitives/Texture.cpp

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

            Objects/Cameras/Camera.cpp
            Objects/Cameras/FirstPersonCamera.cpp
            Objects/Cameras/ThirdPersonCamera.cpp

            Objects/Lenses/Lens.cpp
            Objects/Lenses/PerspectiveLens.cpp

            Objects/Entity.cpp
            Objects/Light.cpp
            Objects/Material.cpp
            Objects/Quad.cpp
            Objects/Skybox.cpp
            Objects/Terrain.cpp

            Core/Initialiser.cpp
            Core/Layer.cpp
            Core/LayerStack.cpp
            Core/Scene.cpp
            Core/Window.cpp

            Events/Event.cpp
            Events/WindowEvent.cpp
            Events/KeyboardEvent.cpp
            Events/MouseEvent.cpp
            
            Utility/FramerateTimer.cpp
            Utility/Log.cpp
            Utility/Maths.cpp
            Utility/Tokenize.cpp)

target_link_libraries(Sprocket PUBLIC
                      spdlog::spdlog
                      spdlog::spdlog_header_only
                      glfw
                      glad::glad
                      glm
                      freetype)

target_include_directories(Sprocket PUBLIC
                           .
                           ./Core
                           ./Display
                           ./Display/Fonts
                           ./Display/WidgetProperties
                           ./Events
                           ./Graphics
                           ./Graphics/Primitives
                           ./Graphics/Rendering
                           ./Graphics/PostProcessing
                           ./Objects
                           ./Objects/Cameras
                           ./Objects/Lenses
                           ./Utility
                           ${STB_INCLUDE_DIR})