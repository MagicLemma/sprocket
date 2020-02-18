#include "Log.h"
#include "Window.h"
#include "Loader.h"
#include "Renderer.h"
#include "RawModel.h"

int main(int argc, char* argv[])
{
    Sprocket::Log::init();
    SPKT_LOG_INFO("Version {}.{}.{}", 0, 0, 1);

    Sprocket::Window window;
    Sprocket::Loader loader;
    Sprocket::Renderer renderer;

    Sprocket::RawModel quad = loader.load({
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},

        {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f}},
    });

    while(window.running()) {
        window.clear();
        window.onUpdate();

        renderer.render(quad);
    }

    return 0;
}