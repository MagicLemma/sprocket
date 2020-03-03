#include "Log.h"
#include "Window.h"
#include "Loader.h"
#include "Renderer.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Layer.h"
#include "Scene.h"
#include "LayerStack.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Core/Initialiser.h"

#include <vector>
#include <memory>

namespace Sprocket {

struct BasicSceneInfo
{
    Camera   camera;
    Shader   shader;
    Shader   shaderMenu;

    std::vector<Entity> entities;
    std::vector<Light>  lights;
    std::vector<Model>  models;

    bool paused;

    BasicSceneInfo(const std::string& vertShader,
                   const std::string& fragShader,
                   const std::string& vertShaderMenu,
                   const std::string& fragShaderMenu)
        : camera()
        , shader(vertShader, fragShader)
        , shaderMenu(vertShaderMenu, fragShaderMenu)
        , entities()
        , lights()
        , models()
        , paused(false) {}
};

class GameLayer : public Layer
{
    std::shared_ptr<BasicSceneInfo> d_info;

public:
    GameLayer(std::shared_ptr<BasicSceneInfo> info) 
        : Layer(Status::NORMAL, false) 
        , d_info(info)
    {
        Model quadModel = Loader::loadModel("Resources/Models/Plane.obj");
        Model dragonModel = Loader::loadModel("Resources/Models/Dragon.obj");

        Texture space = Loader::loadTexture("Resources/Textures/PlainGray.PNG");
        Texture gray = Loader::loadTexture("Resources/Textures/PlainGray.PNG");
        gray.reflectivity(3);
        gray.shineDamper(5);

        d_info->entities.push_back(Entity(dragonModel, gray, {0.0f, 0.0f, -1.0f}, Maths::vec3(0.0f), 0.1f));
        d_info->entities.push_back(Entity(quadModel, space, {0.0f, -1.0f, 0.0f}, Maths::vec3(0.0f), 20));
    
        d_info->lights.push_back(Light{{0.0f, 50.0f, 0.0f}, {0.5f, 0.4f, 0.4f}, {1.0f, 0.0f, 0.0f}});
        d_info->lights.push_back(Light{{5.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.3f, 0.0f}});
        d_info->lights.push_back(Light{{-5.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.3f, 0.0f}});
        d_info->lights.push_back(Light{{8.0f, 4.0f, 2.0f}, {0.3f, 0.8f, 0.2f}, {1.0f, 0.3f, 0.0f}});
    }

    bool handleEventImpl(Window* window, const Event& event) override
    {
        return false;
    }

    void updateImpl(Window* window) override
    {
        d_status = d_info->paused ? Status::PAUSED : Status::NORMAL;

        if (d_status == Status::NORMAL) {
            double tick = layerTicker();

            d_info->lights[1].position.z = 5 * std::sin(tick);
            d_info->lights[1].position.x = 5 * std::cos(tick);

            d_info->lights[2].position.z = 6 * std::sin(-1.5f * tick);
            d_info->lights[2].position.x = 6 * std::cos(-1.5f * tick);

            d_info->lights[3].position.z = 6 * std::sin(8.0f * tick);
            d_info->lights[3].position.x = 6 * std::cos(8.0f * tick);
        }

        d_info->camera.move(window, d_status == Status::NORMAL);
    }

    void drawImpl(Window* window) override
    {
        RenderOptions options;
        options.wireframe = window->isKeyDown(Keyboard::F);

        for (const auto& entity: d_info->entities) {
            Renderer::render(entity,
                             d_info->lights,
                             d_info->camera,
                             d_info->shader,
                             window,
                             options);
        }
    }
};

class UILayer : public Layer
{
    std::shared_ptr<BasicSceneInfo> d_info;

public:
    UILayer(std::shared_ptr<BasicSceneInfo> info) 
        : Layer(Status::INACTIVE, true)
        , d_info(info)
    {
        Vertex2DBuffer v = {{0.5f, 0.5f}, {-0.5f, -0.5f}, {-0.5f, 0.5f},
                        {0.5f, 0.5f}, {0.5f, -0.5f}, {-0.5f, -0.5f}};
        Model tri = Loader::load2DModel(v);
        d_info->models.push_back(tri);
    }

    bool handleEventImpl(Window* window, const Event& event) override
    {
        if (auto e = event.as<Sprocket::KeyboardButtonPressedEvent>()) {
            if (e->key() == Sprocket::Keyboard::ESC) {
                d_info->paused = !d_info->paused;
                window->setCursorVisibility(d_info->paused);
                return true;
            }
        }
        else if (auto e = event.as<MouseButtonPressedEvent>()) {
            auto pos = window->getMousePos();
            if (pos.x < 50) {
                d_info->paused = false;
                window->setCursorVisibility(false);
            }
        }
        return false;
    }

    void updateImpl(Window* window) override
    {
        d_status = d_info->paused ? Status::NORMAL : Status::INACTIVE;
    }

    void drawImpl(Window* window) override
    {
        Sprocket::RenderOptions options;
        options.faceCulling = false;
        for (const auto& model: d_info->models) {
            Renderer::render(model, d_info->shaderMenu, options);
        }
    }
};

}

int main(int argc, char* argv[])
{
    Sprocket::Initialiser init;

    Sprocket::Window window;

    auto info = std::make_shared<Sprocket::BasicSceneInfo>(
        "Resources/Shaders/Basic.vert",
        "Resources/Shaders/Basic.frag",
        "Resources/Shaders/GUI.vert",
        "Resources/Shaders/GUI.frag"
    );

    Sprocket::LayerStack layerStack;
    layerStack.pushLayer(std::make_shared<Sprocket::GameLayer>(info));
    layerStack.pushLayer(std::make_shared<Sprocket::UILayer>(info));

    Sprocket::Scene scene("Scene", layerStack, &window);

    while (window.running()) {
        window.clear();
        scene.tick();
        window.onUpdate();
    }

    return 0;
}