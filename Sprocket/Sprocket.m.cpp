#include "Log.h"
#include "Window.h"
#include "Loader.h"
#include "Graphics/Modelling/Model2D.h"
#include "Graphics/Modelling/Model3D.h"
#include "Texture.h"
#include "FirstPersonCamera.h"
#include "Light.h"
#include "Layer.h"
#include "Scene.h"
#include "LayerStack.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/WindowEvent.h"
#include "Core/Initialiser.h"
#include "Terrain.h"
#include "EntityRenderer.h"
#include "TerrainRenderer.h"
#include "DisplayRenderer.h"
#include "SkyboxRenderer.h"

#include "PostProcessor.h"
#include "Effect.h"
#include "GaussianBlur.h"
#include "Negative.h"

#include <GLFW/glfw3.h>

#include <random>

#include <vector>
#include <memory>

namespace Sprocket {

struct BasicSceneInfo
{
    Window* window;
    FirstPersonCamera  camera;

    Skybox skybox;
 
    std::vector<Entity>   entities;
    std::vector<Terrain>  terrains;
    std::vector<Light>    lights;

    std::vector<std::shared_ptr<Model2D>> models;
    
    bool paused = false;
    
    BasicSceneInfo(Window* window)
        : window(window)
        , skybox({"Resources/Textures/Skybox/Skybox_X_Pos.png",
                  "Resources/Textures/Skybox/Skybox_X_Neg.png",
                  "Resources/Textures/Skybox/Skybox_Y_Pos.png",
                  "Resources/Textures/Skybox/Skybox_Y_Neg.png",
                  "Resources/Textures/Skybox/Skybox_Z_Pos.png",
                  "Resources/Textures/Skybox/Skybox_Z_Neg.png"})
    {}
};

class GameLayer : public Layer
{
    std::shared_ptr<BasicSceneInfo> d_info;

    EntityRenderer  d_entityRenderer;
    TerrainRenderer d_terrainRenderer;
    SkyboxRenderer  d_skyboxRenderer;

    PostProcessor d_postProcessor;

public:
    GameLayer(std::shared_ptr<BasicSceneInfo> info) 
        : Layer(Status::NORMAL, false) 
        , d_info(info)
        , d_entityRenderer(info->window)
        , d_terrainRenderer(info->window)
        , d_skyboxRenderer(info->window)
        , d_postProcessor(info->window->width(), info->window->height())
    {
        auto quadModel = Loader::loadModel3D("Resources/Models/Plane.obj");
        auto dragonModel = Loader::loadModel3D("Resources/Models/Dragon.obj");
        auto deagleModel = Loader::loadModel3D("Resources/Models/Deagle.obj");
        auto cube = Loader::loadModel3D("Resources/Models/Cube.obj");

        auto green = Loader::loadTexture("Resources/Textures/Green.PNG");
        auto space = Loader::loadTexture("Resources/Textures/Space.PNG");
        auto gray = Loader::loadTexture("Resources/Textures/PlainGray.PNG");
        auto shinyGray = Loader::loadTexture("Resources/Textures/PlainGray.PNG");
        
        shinyGray->reflectivity(3);
        shinyGray->shineDamper(5);

        // Make the huge terrain.
        d_info->terrains.push_back(Terrain(green, {0.0f, 0.0f, 0.0f}));
        d_info->terrains.push_back(Terrain(green, {-50.0f, 0.0f, 0.0f}));
        d_info->terrains.push_back(Terrain(green, {0.0f, 0.0f, -50.0f}));
        d_info->terrains.push_back(Terrain(green, {-50.0f, 0.0f, -50.0f}));

        // Load complex models
        d_info->entities.push_back(Entity(dragonModel, shinyGray, {0.0f, 0.0f, -1.0f}, Maths::vec3(0.0f), 0.1f));
        d_info->entities.push_back(Entity(deagleModel, shinyGray, {0.0f, 2.0f, 0.0f}, {180.0f, 0.0f, 0.0f}, 1));
        
        // Load cubes to show the grid.
        d_info->entities.push_back(Entity(cube, space, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.2));
        d_info->entities.push_back(Entity(cube, space, {5.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.2));
        d_info->entities.push_back(Entity(cube, space, {10.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, 0.2));
        d_info->entities.push_back(Entity(cube, space, {0.0f, 0.0f, 5.0f}, {0.0f, 0.0f, 0.0f}, 0.2));

        // Seed with a real random value, if available
        std::random_device r;
    
        // Choose a random mean between 1 and 6
        std::default_random_engine e1(r());
        std::uniform_real_distribution<float> udist(-50, 50);
        std::uniform_real_distribution<float> urot(-180.0f, 180.0f);

        // Load a bunch of random cubes/
        for (int i = 0; i != 50; ++i) {
            d_info->entities.push_back(Entity{cube, shinyGray, {udist(e1), 50 + udist(e1), udist(e1)}, {urot(e1), urot(e1), urot(e1)}, 0.5f});
        }

        d_info->window->setCursorVisibility(false);

        d_info->lights.push_back(Light{{0.0f, 50.0f, 0.0f}, {0.5f, 0.4f, 0.4f}, {1.0f, 0.0f, 0.0f}});
        d_info->lights.push_back(Light{{5.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
        d_info->lights.push_back(Light{{-5.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});
        d_info->lights.push_back(Light{{8.0f, 4.0f, 2.0f}, {0.3f, 0.8f, 0.2f}, {1.0f, 0.0f, 0.0f}});

        //d_postProcessor.addEffect<Negative>();
        d_postProcessor.addEffect<GaussianVert>();
        d_postProcessor.addEffect<GaussianHoriz>();
    }

    bool handleEventImpl(Window* window, const Event& event) override
    {
        if (auto e = event.as<WindowResizeEvent>()) {
            d_postProcessor.setScreenSize(e->width(), e->height());
        }
        return false;
    }

    void updateImpl(Window* window) override
    {
        d_status = d_info->paused ? Status::PAUSED : Status::NORMAL;

        if (d_status == Status::NORMAL) {
            double tick = layerTicker();

            d_info->lights[1].position.z = 50 * std::sin(tick);
            d_info->lights[1].position.x = 50 * std::cos(tick);

            d_info->lights[2].position.z = 60 * std::sin(-1.5f * tick);
            d_info->lights[2].position.x = 60 * std::cos(-1.5f * tick);

            d_info->lights[3].position.z = 60 * std::sin(8.0f * tick);
            d_info->lights[3].position.x = 60 * std::cos(8.0f * tick);

            d_info->camera.update(window, deltaTime());

            window->setCursorVisibility(false);
        }
        else {
            window->setCursorVisibility(true);
        }
        
    }

    void drawImpl(Window* window) override
    {
        if(d_info->paused) {
            d_postProcessor.bind();
        }
        
        RenderOptions options;
        options.wireframe = window->isKeyDown(Keyboard::F) &&
                            d_status == Status::NORMAL;

        
        d_entityRenderer.update(d_info->camera, d_info->lights, options);
        d_terrainRenderer.update(d_info->camera, d_info->lights, options);

        for (const auto& entity: d_info->entities) {
            d_entityRenderer.draw(entity);
        }

        for (const auto& terrain: d_info->terrains) {
            d_terrainRenderer.draw(terrain);
        }
        d_skyboxRenderer.draw(d_info->skybox,
                              d_info->camera);
        
        
        if (d_info->paused) {
            d_postProcessor.unbind();
            d_postProcessor.draw();
        }
    }
};

class UILayer : public Layer
{
    std::shared_ptr<BasicSceneInfo> d_info;

    DisplayRenderer d_displayRenderer;

public:
    UILayer(std::shared_ptr<BasicSceneInfo> info) 
        : Layer(Status::INACTIVE, true)
        , d_info(info)
        , d_displayRenderer(info->window)
    {
        Vertex2DBuffer v = {
            {Maths::vec2{0.5f, 0.5f}, Maths::vec2{1.0f, 1.0f}},
            {Maths::vec2{-0.5f, -0.5f}, Maths::vec2{0.0f, 0.0f}},
            {Maths::vec2{-0.5f, 0.5f}, Maths::vec2{0.0f, 1.0f}},
            {Maths::vec2{0.5f, 0.5f}, Maths::vec2{1.0f, 1.0f}},
            {Maths::vec2{0.5f, -0.5f}, Maths::vec2{1.0f, 0.0f}},
            {Maths::vec2{-0.5f, -0.5f}, Maths::vec2{0.0f, 0.0f}}
        };
        auto tri = Loader::loadModel2D(v);
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
        options.depthTest = false;
        for (const auto& model: d_info->models) {
            d_displayRenderer.draw(model, options);
        }
    }
};

}

int main(int argc, char* argv[])
{
    Sprocket::Initialiser init;
    Sprocket::Window window;

    auto info = std::make_shared<Sprocket::BasicSceneInfo>(&window);

    Sprocket::LayerStack layerStack;
    layerStack.pushLayer(std::make_shared<Sprocket::GameLayer>(info));
    layerStack.pushLayer(std::make_shared<Sprocket::UILayer>(info));

    Sprocket::Scene scene("Scene", layerStack, &window);

    double previousTime = glfwGetTime();
    int frameCount = 0;

    while (window.running()) {
        window.clear();
        scene.tick();
        window.onUpdate();

        // Print framerate
        double currentTime = glfwGetTime();
        frameCount++;
        // If a second has passed.
        if ( currentTime - previousTime >= 1.0 )
        {
            // Display the frame count here any way you want.
            SPKT_LOG_INFO("{}", frameCount);

            frameCount = 0;
            previousTime = currentTime;
        }
    }

    return 0;
}