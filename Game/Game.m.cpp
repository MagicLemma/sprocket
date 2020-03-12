#include <Sprocket.h>

#include "WorldSceneInfo.h"
#include "WorldLayer.h"
#include "UILayer.h"

int main()
{
    Sprocket::Initialiser init;
    Sprocket::Window window;

    auto info = std::make_shared<BasicSceneInfo>(&window);

    Sprocket::LayerStack layerStack;
    layerStack.pushLayer(std::make_shared<WorldLayer>(info));
    layerStack.pushLayer(std::make_shared<UILayer>(info));

    Sprocket::Scene scene("Scene", layerStack, &window);

    Sprocket::FramerateTimer fps; // Helper class to log the fps.

    while (window.running()) {
        window.clear();
        scene.tick();
        window.onUpdate();

        fps.update();
    }

    return 0;
}
