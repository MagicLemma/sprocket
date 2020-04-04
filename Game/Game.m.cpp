#include <Sprocket.h>
#include "MainMenu.h"
#include "WorldSceneInfo.h"
#include "UILayer.h"
#include "WorldLayer.h"

int main()
{
    Sprocket::Initialiser init;
    Sprocket::Window window;
    Sprocket::ResourceManager resourceManager;
    Sprocket::SceneManager sceneManager;

    window.setCallback([&sceneManager](const Sprocket::Event& event) {
        sceneManager.handleEvent(event);
    });

    Sprocket::Accessor accessor(&sceneManager, &resourceManager, &window);

    auto world = sceneManager.addScene("World");
    auto info = std::make_shared<BasicSceneInfo>(accessor);
    world->add<WorldLayer>(accessor, info);
    world->add<UILayer>(accessor, info);
    sceneManager.setActiveScene("World");

    Sprocket::FramerateTimer fps; // Helper class to log the fps.

    while (window.running()) {
        window.clear();

        sceneManager.update();
        sceneManager.draw();

        window.onUpdate();
        fps.update();
    }

    return 0;
}
