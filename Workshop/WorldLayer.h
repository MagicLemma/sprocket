#pragma once
#include <Sprocket.h>

#include <memory>
#include <random>

enum class Mode { PLAYER, OBSERVER, EDITOR };

class WorldLayer : public Sprocket::Layer
{
    Mode d_mode;

    // PLAYER MODE
    Sprocket::Entity d_playerCamera;
    Sprocket::Entity d_observerCamera;
    Sprocket::Entity d_editorCamera;

    Sprocket::Entity d_activeCamera;
    
    // RENDERING
    Sprocket::EntityRenderer  d_entityRenderer;
    Sprocket::SkyboxRenderer  d_skyboxRenderer;

    Sprocket::PostProcessor   d_postProcessor;

    // WORLD
    // Entity management and systems
    std::shared_ptr<Sprocket::PhysicsEngine> d_physicsEngine;
    std::shared_ptr<Sprocket::Selector>      d_selector;
    std::shared_ptr<Sprocket::CameraSystem>  d_cameraSystem;
    std::shared_ptr<Sprocket::ScriptRunner>  d_scriptRunner;
    
    std::shared_ptr<Sprocket::Scene> d_scene;

    // Additional world setup
    Sprocket::Skybox d_skybox;
    Sprocket::Lights d_lights;
    float            d_sunAngle = 45.0f;
    
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