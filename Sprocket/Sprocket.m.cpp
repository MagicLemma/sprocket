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
    gray.reflectivity(3);
    gray.shineDamper(5);

    Sprocket::Entity dragon(dragonModel, gray, {0.0f, 0.0f, -1.0f}, glm::vec3(0.0f), 0.1f);
    Sprocket::Entity quad(quadModel, space, {0.0f, -1.0f, 0.0f}, glm::vec3(0.0f), 20);

    Sprocket::Light sun{{0.0f, 50.0f, 0.0f}, {0.5f, 0.4f, 0.4f}, {1.0f, 0.0f, 0.0f}};
    Sprocket::Light light1{{5.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.3f, 0.0f}};
    Sprocket::Light light2{{-5.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.3f, 0.0f}};
    Sprocket::Light light3{{8.0f, 4.0f, 2.0f}, {0.3f, 0.8f, 0.2f}, {1.0f, 0.3f, 0.0f}};
    std::vector<Sprocket::Light> lights = {sun, light1, light2, light3};


    Sprocket::Camera camera;

    float r = 0.0f;
    while (window.running()) {
        window.clear();
        camera.move();

        //dragon.increaseRotation(0.0f, 0.5f, 0.0f);

        renderer.render(dragon, lights, camera, shader);
        renderer.render(quad, lights, camera, shader);

        window.onUpdate();
        r += 0.05f;
    }

    return 0;
}