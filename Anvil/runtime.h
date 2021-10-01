#pragma once
#include <Sprocket/Graphics/asset_manager.h>
#include <Sprocket/Graphics/CubeMap.h>
#include <Sprocket/Graphics/Rendering/Scene3DRenderer.h>
#include <Sprocket/Graphics/Rendering/SkyboxRenderer.h>
#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Scene/scene.h>
#include <Sprocket/UI/console.h>

#include <memory>
#include <random>

namespace spkt {
    class window;
}

class Runtime
{
    spkt::window*      d_window;
    spkt::asset_manager d_assetManager;

    // Rendering
    spkt::Scene3DRenderer d_entityRenderer;
    spkt::SkyboxRenderer d_skyboxRenderer;

    // Scene
    spkt::scene d_scene;
    spkt::CubeMap d_skybox;
    
    spkt::entity d_runtimeCamera;

    // Console
    spkt::SimpleUI d_ui;
    spkt::console  d_console;
    std::string    d_command_line;
    bool           d_consoleActive = false;

public:
    Runtime(spkt::window* window);

    void on_event(spkt::event& event);
    void on_update(double dt);
    void on_render();
};