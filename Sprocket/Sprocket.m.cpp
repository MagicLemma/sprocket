#include "Log.h"
#include "Window.h"
#include "Keyboard.h"
#include "Loader.h"
#include "Renderer.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"

int main(int argc, char* argv[])
{
    Sprocket::Log::init();
    SPKT_LOG_INFO("Version {}.{}.{}", 0, 0, 1);

    Sprocket::Window window;
    Sprocket::Loader loader;
    Sprocket::Renderer renderer;

    Sprocket::Keyboard::init(&window);

    Sprocket::Shader shader("Resources/Shaders/Basic.vert",
                            "Resources/Shaders/Basic.frag");

    shader.loadProjectionMatrix(window.aspectRatio(), 70.0f, 0.1f, 1000.0f);

    Sprocket::Model quadModel = loader.loadModel("Resources/Models/Plane.obj");
    Sprocket::Model dragonModel = loader.loadModel("Resources/Models/Dragon.obj");

    Sprocket::Texture space = loader.loadTexture("Resources/Textures/Space.PNG");
    Sprocket::Texture gray = loader.loadTexture("Resources/Textures/PlainGray.PNG");
    gray.reflectivity(1);
    gray.shineDamper(10);

    Sprocket::Entity dragon(dragonModel, gray, {0.0f, 0.0f, -1.0f}, glm::vec3(0.0f), 0.1f);
    Sprocket::Entity quad(quadModel, space, {0.0f, -1.0f, 0.0f}, glm::vec3(0.0f), 20);

    Sprocket::Light sun{{0.0f, 50.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}};
    Sprocket::Light light{{0.0f, 1.0f, 0.0f}, {0.8f, 0.8f, 0.9f}, {1.0f, 0.0f, 0.0f}};

    Sprocket::Camera camera;

    float r = 0.0f;
    while (window.running()) {
        window.clear();
        camera.move();

        light.position = glm::vec3(3*std::sin(r), 1.0f, 3*std::cos(r));
        dragon.increaseRotation(0.0f, 0.5f, 0.0f);

        renderer.render(dragon, sun, camera, shader);
        renderer.render(quad, sun, camera, shader);

        window.onUpdate();
        r += 0.1f ;
    }

    return 0;
}