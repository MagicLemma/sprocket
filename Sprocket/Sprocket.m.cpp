#include "Log.h"
#include "Window.h"
#include "Keyboard.h"
#include "Loader.h"
#include "Renderer.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"

#include <cmath>

int main(int argc, char* argv[])
{
    Sprocket::Log::init();
    SPKT_LOG_INFO("Version {}.{}.{}", 0, 0, 1);

    Sprocket::Window window;
    Sprocket::Loader loader;
    Sprocket::Renderer renderer;

    Sprocket::Keyboard::init(&window);

    Sprocket::Loader::VertexBuffer vb{
        {{-0.5f,0.5f,-0.5f},  {0.0f, 0.0f}},
        {{-0.5f,-0.5f,-0.5f}, {0.0f, 1.0f}},
        {{0.5f,-0.5f,-0.5f},  {1.0f, 1.0f}},
        {{0.5f,0.5f,-0.5f},   {1.0f, 0.0f}},

        {{-0.5f,0.5f,0.5f},  {0.0f, 0.0f}},
        {{-0.5f,-0.5f,0.5f}, {0.0f, 1.0f}},
        {{0.5f,-0.5f,0.5f},  {1.0f, 1.0f}},
        {{0.5f,0.5f,0.5f},   {1.0f, 0.0f}},

        {{0.5f,0.5f,-0.5f},  {0.0f, 0.0f}},
        {{0.5f,-0.5f,-0.5f}, {0.0f, 1.0f}},
        {{0.5f,-0.5f,0.5f},  {1.0f, 1.0f}},
        {{0.5f,0.5f,0.5f},   {1.0f, 0.0f}},

        {{-0.5f,0.5f,-0.5f},  {0.0f, 0.0f}},
        {{-0.5f,-0.5f,-0.5f}, {0.0f, 1.0f}},
        {{-0.5f,-0.5f,0.5f},  {1.0f, 1.0f}},
        {{-0.5f,0.5f,0.5f},   {1.0f, 0.0f}},

        {{-0.5f,0.5f,0.5f},  {0.0f, 0.0f}},
        {{-0.5f,0.5f,-0.5f}, {0.0f, 1.0f}},
        {{0.5f,0.5f,-0.5f},  {1.0f, 1.0f}},
        {{0.5f,0.5f,0.5f},   {1.0f, 0.0f}},

        {{-0.5f,-0.5f,0.5f},  {0.0f, 0.0f}},
        {{-0.5f,-0.5f,-0.5f}, {0.0f, 1.0f}},
        {{0.5f,-0.5f,-0.5f},  {1.0f, 1.0f}},
        {{0.5f,-0.5f,0.5f},   {1.0f, 0.0f}},
    };

    Sprocket::Loader::IndexBuffer ib{
        0,1,3,	
        3,1,2,	
        4,5,7,
        7,5,6,
        8,9,11,
        11,9,10,
        12,13,15,
        15,13,14,	
        16,17,19,
        19,17,18,
        20,21,23,
        23,21,22
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

    Sprocket::Entity entity2(quad,
                             texture,
                             glm::vec3(0.0f, -12.0f, 0.0f),
                             glm::vec3(0.0),
                             20);

    Sprocket::Camera camera;

    while(window.running()) {
        window.clear();

        entity.increaseRotation(0.5f, 0.0f, 0.0f);
        camera.move();

        renderer.render(entity, camera, shader);
        renderer.render(entity2, camera, shader);
        SPKT_LOG_INFO("Pitch: {}", camera.pitch());
        
        window.onUpdate();
    }

    return 0;
}