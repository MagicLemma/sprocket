#include "Log.h"
#include "Window.h"
#include "Loader.h"
#include "Renderer.h"
#include "RawModel.h"
#include "Shader.h"

int main(int argc, char* argv[])
{
    Sprocket::Log::init();
    SPKT_LOG_INFO("Version {}.{}.{}", 0, 0, 1);

    Sprocket::Window window;
    Sprocket::Loader loader;
    Sprocket::Renderer renderer;

    Sprocket::Loader::VertexBuffer vb{
        {{-0.5f, 0.5f, 0.0f},  {0.0f, 1.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f},  {1.0f, 1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f},   {1.0f, 1.0f, 1.0f}}
    };

    Sprocket::Loader::IndexBuffer ib{
        0, 1, 3,
        3, 1, 2
    };

    Sprocket::Shader shader("Sprocket/Shaders/Basic.vert",
                            "Sprocket/Shaders/Basic.frag");

    Sprocket::RawModel quad = loader.load(vb, ib);

    while(window.running()) {
        window.clear();

        shader.start();
        renderer.render(quad);
        shader.stop();

        window.onUpdate();
    }

    return 0;
}