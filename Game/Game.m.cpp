#include <Sprocket.h>
#include "MainMenu.h"
#include "UILayer.h"
#include "WorldLayer.h"

int main()
{
    Sprocket::Initialiser init;
    Sprocket::Window window;
    Sprocket::SceneManager sceneManager;

    window.setCallback([&sceneManager](const Sprocket::Event& event) {
        sceneManager.handleEvent(event);
    });

    Sprocket::Accessor accessor(&sceneManager, &window);

    auto world = sceneManager.addScene("World");
    auto worldLayer = world->add<WorldLayer>(accessor);
    world->add<UILayer>(accessor, worldLayer.get());
    sceneManager.setActiveScene("World");

    //Sprocket::FramerateTimer fps; // Helper class to log the fps.

    while (window.running()) {
        window.clear();

        sceneManager.update();
        sceneManager.draw();

        window.onUpdate();
        //fps.update();
    }

    return 0;
}
