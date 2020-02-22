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

    Sprocket::Model plane = loader.loadModel("Resources/Models/Plane.obj");
    Sprocket::Model dragon = loader.loadModel("Resources/Models/Dragon.obj");

    Sprocket::Texture texture = loader.loadTexture("Resources/Textures/Space.PNG");
    Sprocket::Texture steel = loader.loadTexture("Resources/Textures/PlainGray.PNG");

    Sprocket::Entity pistol(dragon, steel, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f), 0.1f);
    pistol.increaseRotation(180.0f, 0.0f, 0.0f);

    Sprocket::Entity floor(plane, steel, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f), 20);

    Sprocket::Light light{
        {0.0f, 1.0f, 0.0f},
        {0.8f, 0.8f, 0.9f}
    };
    shader.loadLight(light);

    Sprocket::Camera camera;

    float r = 0.0f;
    pistol.increaseRotation(180.0f, 0.0f, 0.0f);
    while(window.running()) {
        window.clear();

        camera.move();
        shader.bind();
        light.position = glm::vec3(3*std::sin(r), 1.0f, 3*std::cos(r));
        shader.loadLight(light);
        pistol.increaseRotation(0.0f, -1.5f, 0.0f);
        renderer.render(pistol, camera, shader);
        renderer.render(floor, camera, shader);
        shader.unbind();
        window.onUpdate();
        r += 0.1f ;
    }

    return 0;
}