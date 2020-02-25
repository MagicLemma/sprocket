#include "Log.h"
#include "Window.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Loader.h"
#include "Renderer.h"
#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Layer.h"
#include "Scene.h"
#include "SceneData.h"
#include "LayerStack.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

#include "Model2D.h"
#include "Loader2D.h"
#include "Renderer2D.h"
#include "Shader2D.h"
#include "Vertex2D.h"

#include <vector>
#include <memory>

namespace Sprocket {

class GameLayer : public Layer
{
    Loader   d_loader;
    Renderer d_renderer;
    Camera   d_camera;
    Shader   d_shader;

    std::vector<Entity> d_entities;
    std::vector<Light>  d_lights;

public:
    GameLayer(Window* window) 
        : Layer(Status::NORMAL, false) 
        , d_loader()
        , d_renderer()
        , d_camera()
        , d_shader("Resources/Shaders/Basic.vert",
                   "Resources/Shaders/Basic.frag")
        , d_entities()
        , d_lights()
    {
        d_shader.loadProjectionMatrix(window->aspectRatio(), 70.0f, 0.1f, 1000.0f);
        
        Model quadModel = d_loader.loadModel("Resources/Models/Plane.obj");
        Model dragonModel = d_loader.loadModel("Resources/Models/Dragon.obj");

        Texture space = d_loader.loadTexture("Resources/Textures/PlainGray.PNG");
        Texture gray = d_loader.loadTexture("Resources/Textures/PlainGray.PNG");
        gray.reflectivity(3);
        gray.shineDamper(5);
        //space.reflectivity(3);
        //space.shineDamper(5);

        d_entities.push_back(Entity(dragonModel, gray, {0.0f, 0.0f, -1.0f}, Maths::vec3(0.0f), 0.1f));
        d_entities.push_back(Entity(quadModel, space, {0.0f, -1.0f, 0.0f}, Maths::vec3(0.0f), 20));
    
        d_lights.push_back(Light{{0.0f, 50.0f, 0.0f}, {0.5f, 0.4f, 0.4f}, {1.0f, 0.0f, 0.0f}});
        d_lights.push_back(Light{{5.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.3f, 0.0f}});
        d_lights.push_back(Light{{-5.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.3f, 0.0f}});
        d_lights.push_back(Light{{8.0f, 4.0f, 2.0f}, {0.3f, 0.8f, 0.2f}, {1.0f, 0.3f, 0.0f}});
    }

    bool handleEventImpl(const Event& event, SceneData* data) override
    {
        return false;
    }

    void updateImpl(SceneData* data) override
    {
        d_status = data->paused() ? Status::PAUSED : Status::NORMAL;

        if (d_status == Status::NORMAL) {
            float tick = layerTicker();

            d_lights[1].position.z = 5 * std::sin(tick);
            d_lights[1].position.x = 5 * std::cos(tick);

            d_lights[2].position.z = 6 * std::sin(-1.5f * tick);
            d_lights[2].position.x = 6 * std::cos(-1.5f * tick);

            d_lights[3].position.z = 6 * std::sin(8.0f * tick);
            d_lights[3].position.x = 6 * std::cos(8.0f * tick);
        }

        d_camera.move(d_status == Status::NORMAL);
    }

    void drawImpl() override
    {
        for (const auto& entity: d_entities) {
            d_renderer.render(entity, d_lights, d_camera, d_shader);
        }
    }
};

class UILayer : public Layer
{
    Loader2D   d_loader;
    Renderer2D d_renderer;
    Shader2D   d_shader;

    std::vector<Model2D> d_models;

public:
    UILayer() 
        : Layer(Status::INACTIVE, true)
        , d_loader()
        , d_renderer()
        , d_shader("Resources/Shaders/GUI.vert",
                   "Resources/Shaders/GUI.frag")
    {
        Vertex2DBuffer v = {{0.5f, 0.5f}, {-0.5f, 0.5f}, {-0.5f, -0.5f},
                        {0.5f, 0.5f}, {-0.5f, -0.5f}, {0.5f, -0.5f}};
        Model2D tri = d_loader.load2DModel(v);
        d_models.push_back(tri);
    }

    bool handleEventImpl(const Event& event, SceneData* data) override
    {
        if (auto e = event.as<MouseButtonPressedEvent>()) {
            auto pos = Mouse::getMousePos();
            if (pos.x < 50) {
                data->paused(false);
                data->window()->setCursorVisibility(false);
            }
        }
        return false;
    }

    void updateImpl(SceneData* data) override
    {
        d_status = data->paused() ? Status::NORMAL : Status::INACTIVE;

    }

    void drawImpl() override
    {
        for (const auto& model: d_models) {
            d_renderer.render(model, d_shader);
        }
    }
};

}

int main(int argc, char* argv[])
{
    Sprocket::Log::init();
    SPKT_LOG_INFO("Version {}.{}.{}", 0, 0, 1);

    Sprocket::Window window;
    Sprocket::Keyboard::init(&window);
    Sprocket::Mouse::init(&window);

    Sprocket::LayerStack layerStack;
    layerStack.pushLayer(std::make_shared<Sprocket::GameLayer>(&window));
    layerStack.pushLayer(std::make_shared<Sprocket::UILayer>());

    Sprocket::SceneData sceneData(
        "Sprocket",
        Sprocket::SceneType::STAGE,
        &window);

    Sprocket::Scene scene(sceneData, layerStack,
        [](const Sprocket::Event& event, Sprocket::SceneData* data){
            if (auto e = event.as<Sprocket::KeyboardButtonPressedEvent>()) {
                if (e->key() == Sprocket::Keyboard::ESC) {
                    data->paused(!data->paused());
                    data->window()->setCursorVisibility(data->paused());
                }
            }
        });

    while (window.running()) {
        window.clear();
        scene.tick();
        window.onUpdate();
    }

    return 0;
}