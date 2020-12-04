#pragma once
#include <Sprocket.h>
#include "Console.h"

#include <memory>
#include <random>

namespace Sprocket {

class EditorLayer
{
    CoreSystems d_core;

    // Rendering
    EntityRenderer d_entityRenderer;
    SkyboxRenderer d_skyboxRenderer;

    ParticleManager d_particleManager;

    // Scene
    CubeMap d_skybox;
    std::shared_ptr<Scene> d_scene;
    Entity d_runtimeCamera;

    // Console
    Console d_console;
    bool d_consoleActive = false;

public:
    EditorLayer(const CoreSystems& core);

    void OnEvent(Event& event);
    void OnUpdate(double dt);
    void OnRender();

    Window* GetWindow() { return d_core.window; }
};

}