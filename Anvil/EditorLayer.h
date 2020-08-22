#pragma once
#include <Sprocket.h>

#include <memory>
#include <random>

namespace Sprocket {

enum class Mode { PLAYER, OBSERVER, EDITOR };

class WorldLayer : public Layer
{
    Mode d_mode;

    Entity d_camera;
    
    // RENDERING
    EntityRenderer  d_entityRenderer;
    SkyboxRenderer  d_skyboxRenderer;

    PostProcessor   d_postProcessor;

    // WORLD
    // Entity management and systems
    PhysicsEngine  d_physicsEngine;
    Selector       d_selector;
    CameraSystem   d_cameraSystem;
    ScriptRunner   d_scriptRunner;

    Scene          d_scene;

    Serialiser d_serialiser;

    // Additional world setup
    Skybox d_skybox;
    Lights d_lights;
    float  d_sunAngle = 45.0f;
    
    // LAYER DATA
    bool d_paused = false;
    bool d_mouseRequired = false;

    friend class EscapeMenu;
    friend class EditorUI;

public:
    WorldLayer(const Sprocket::CoreSystems& core);

    void OnEvent(Sprocket::Event& event) override;
    void OnUpdate(double dt) override;
};

}