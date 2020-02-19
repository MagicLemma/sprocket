#include "Log.h"
#include "Window.h"
#include "Loader.h"
#include "Renderer.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"

int main(int argc, char* argv[])
{
    Sprocket::Log::init();
    SPKT_LOG_INFO("Version {}.{}.{}", 0, 0, 1);

    Sprocket::Window window;
    Sprocket::Loader loader;
    Sprocket::Renderer renderer;

    Sprocket::Loader::VertexBuffer vb{
        {{-0.5f, 0.5f, 0.0f},  {0.0f, 0.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.0f},  {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f},   {1.0f, 1.0f}}
    };

    Sprocket::Loader::IndexBuffer ib{
        0, 1, 3,
        3, 1, 2
    };

    Sprocket::Loader::TextureFile tex("Resources/Textures/Space.PNG");

    Sprocket::Shader shader("Resources/Shaders/Basic.vert",
                            "Resources/Shaders/Basic.frag");

    Sprocket::Model quad = loader.load(vb, ib);
    Sprocket::Texture texture = loader.loadTexture(tex);

    while(window.running()) {
        window.clear();

        shader.start();
        renderer.render(quad, texture);
        shader.stop();

        window.onUpdate();
    }

    return 0;
}