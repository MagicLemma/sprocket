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

    Sprocket::Shader shader("Resources/Shaders/Basic.vert",
                            "Resources/Shaders/Basic.frag");

    shader.loadProjectionMatrix(window.aspectRatio(), 70.0f, 0.1f, 1000.0f);

    Sprocket::Model plane = loader.loadModel("Resources/Models/Plane.obj");
    Sprocket::Model deagle = loader.loadModel("Resources/Models/Deagle.obj");

    Sprocket::Texture texture = loader.loadTexture("Resources/Textures/Space.PNG");
    Sprocket::Texture steel = loader.loadTexture("Resources/Textures/Steel.PNG");

    Sprocket::Entity pistol(deagle, steel, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f), 0.1);
    pistol.increaseRotation(180.0f, 0.0f, 0.0f);

    Sprocket::Entity floor(plane, texture, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f), 20);

    Sprocket::Camera camera;

    
    while(window.running()) {
        window.clear();

        camera.move();
        
        renderer.render(pistol, camera, shader);
        renderer.render(floor, camera, shader);
        
        window.onUpdate();
    }

    return 0;
}