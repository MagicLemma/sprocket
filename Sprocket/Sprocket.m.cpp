#include "Log.h"
#include "Window.h"
#include "Keyboard.h"
#include "Loader.h"
#include "Renderer.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

int main(int argc, char* argv[])
{
    Sprocket::Log::init();
    SPKT_LOG_INFO("Version {}.{}.{}", 0, 0, 1);

    Sprocket::Window window;
    Sprocket::Loader loader;
    Sprocket::Renderer renderer;

    Sprocket::Keyboard::init(&window);

    Sprocket::Loader::VertexBuffer vb{
        {{-0.5f, 0.5f, 0.0f},  {0.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.0f},  {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f},   {1.0f, 1.0f}}
    };

    Sprocket::Loader::IndexBuffer ib{
        0, 1, 3,
        3, 1, 2
    };

    Sprocket::Loader::TextureFile tex("Resources/Textures/Space.PNG");

    Sprocket::Shader shader("Resources/Shaders/Basic.vert",
                            "Resources/Shaders/Basic.frag");

    shader.loadProjectionMatrix(window.aspectRatio(), 70.0f, 0.1f, 1000.0f);

    Sprocket::Model quad = loader.load(vb, ib);
    Sprocket::Texture texture = loader.loadTexture(tex);

    Sprocket::Entity entity(quad,
                            texture,
                            glm::vec3(0.0f, 0.0f, -1.0f),
                            glm::vec3(0.0),
                            1);

    Sprocket::Camera camera;

    while(window.running()) {
        window.clear();

        entity.increaseRotation(0.5f, 0.0f, 0.0f);
        camera.move();

        renderer.render(entity, camera, shader);
        
        window.onUpdate();
    }

    return 0;
}