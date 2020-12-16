#pragma once
#include <Sprocket.h>
#include "Console.h"

#include <memory>
#include <random>

class Runtime
{
    Sprocket::Window*      d_window;
    Sprocket::AssetManager d_assetManager;

    // Rendering
    Sprocket::EntityRenderer d_entityRenderer;
    Sprocket::SkyboxRenderer d_skyboxRenderer;

    Sprocket::ParticleManager d_particleManager;

    // Scene
    Sprocket::CubeMap d_skybox;
    std::shared_ptr<Sprocket::Scene> d_scene;
    Sprocket::ECS::Entity d_runtimeCamera;

    // Console
    Sprocket::Console d_console;
    bool d_consoleActive = false;

public:
    Runtime(Sprocket::Window* window);

    void OnEvent(Sprocket::Event& event);
    void OnUpdate(double dt);
    void OnRender();
};