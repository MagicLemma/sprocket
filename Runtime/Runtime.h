#pragma once
#include "Console.h"

#include <Sprocket/Graphics/AssetManager.h>
#include <Sprocket/Graphics/Rendering/Scene3DRenderer.h>
#include <Sprocket/Graphics/Rendering/SkyboxRenderer.h>
#include <Sprocket/Scene/scene.h>
#include <Sprocket/Graphics/CubeMap.h>
#include <Sprocket/Scene/ecs.h>

#include <memory>
#include <random>

namespace spkt {
    class Window;
}

class Runtime
{
    spkt::Window*      d_window;
    spkt::AssetManager d_assetManager;

    // Rendering
    spkt::Scene3DRenderer d_entityRenderer;
    spkt::SkyboxRenderer d_skyboxRenderer;

    // Scene
    spkt::scene d_scene;
    spkt::CubeMap d_skybox;
    
    spkt::entity d_runtimeCamera;

    // Console
    Console d_console;
    bool d_consoleActive = false;

public:
    Runtime(spkt::Window* window);

    void on_event(spkt::event& event);
    void on_update(double dt);
    void on_render();
};