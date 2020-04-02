#include <Sprocket.h>

#include "UILayer.h"
#include "MainMenu.h"

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

    auto mainMenu = sceneManager.addScene("Main Menu");
    mainMenu->add<MainMenu>(accessor);
    sceneManager.setActiveScene("Main Menu");

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
