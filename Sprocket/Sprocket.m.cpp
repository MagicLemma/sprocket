#include "Log.h"
#include "RawModel.h"
#include "Vertex.h"
#include "Window.h"
#include "Loader.h"
#include "Renderer.h"
#include "Events/Event.h"

int main(int argc, char* argv[])
{
    Sprocket::Log::init();
    SPKT_LOG_CORE_INFO("Version {}.{}.{}", 0, 0, 1);

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