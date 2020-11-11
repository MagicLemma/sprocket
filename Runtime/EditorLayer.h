#pragma once
#include <Sprocket.h>
#include "Console.h"

#include <memory>
#include <random>

namespace Sprocket {

class EditorLayer : public Layer
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
    bool d_consoleActive = true;

public:
    EditorLayer(const CoreSystems& core);

    void OnEvent(Event& event) override;
    void OnUpdate(double dt) override;

    Window* GetWindow() { return d_core.window; }
};

}