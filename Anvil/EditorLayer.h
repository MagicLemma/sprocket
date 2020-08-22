#pragma once
#include <Sprocket.h>
#include "Camera.h"

#include <memory>
#include <random>

namespace Sprocket {

class EditorLayer : public Layer
{
    Camera d_editorCamera;

    // Rendering
    EntityRenderer d_entityRenderer;
    SkyboxRenderer d_skyboxRenderer;

    FrameBuffer d_viewport;
    DevUI::Context d_ui;
    bool d_isViewportHovered = false;
    bool d_isViewportFocused = false;

    // Scene
    std::string d_sceneFile = "Resources/Anvil.yaml";
    std::shared_ptr<Scene> d_activeScene;
    std::shared_ptr<Scene> d_scene;
    Entity d_runtimeCamera;

    // Additional world setup
    Skybox d_skybox;
    Lights d_lights;
    float  d_sunAngle = 45.0f;
    
    // LAYER DATA
    bool d_paused = false;
    bool d_mouseRequired = false;

    bool d_playingGame = false;

    friend class EscapeMenu;

public:
    EditorLayer(const Sprocket::CoreSystems& core);

    void OnEvent(Sprocket::Event& event) override;
    void OnUpdate(double dt) override;
};

}