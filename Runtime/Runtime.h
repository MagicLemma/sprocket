#pragma once
#include <Sprocket.h>
#include "Console.h"

#include <memory>
#include <random>

class Runtime
{
    spkt::Window*      d_window;
    spkt::AssetManager d_assetManager;

    // Rendering
    spkt::Scene3DRenderer d_entityRenderer;
    spkt::SkyboxRenderer d_skyboxRenderer;

    // Scene
    spkt::Scene d_scene;
    spkt::CubeMap d_skybox;
    
    spkt::entity d_runtimeCamera;

    // Console
    spkt::Console d_console;
    bool d_consoleActive = false;

public:
    Runtime(spkt::Window* window);

    void on_event(spkt::ev::Event& event);
    void on_update(double dt);
    void on_render();
};