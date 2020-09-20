#pragma once
#include <Sprocket.h>
#include "Camera.h"

#include "Inspector.h"

#include <memory>
#include <random>

namespace Sprocket {

class EditorLayer : public Layer
{
    Camera d_editorCamera;

    // Rendering
    EntityRenderer d_entityRenderer;
    SkyboxRenderer d_skyboxRenderer;
    ColliderRenderer d_colliderRenderer;

    ParticleManager d_particleManager;

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
    bool d_showColliders = false;

    void AddEntityToList(const Entity& entity);
    Entity d_selected;

    // Panels
    Inspector      d_inspector;

    void MaterialUI(Texture& texture);

public:
    EditorLayer(const Sprocket::CoreSystems& core);

    void OnEvent(Sprocket::Event& event) override;
    void OnUpdate(double dt) override;

    Entity Selected() { return d_selected; }
    void SetSelected(Entity e) { d_selected = e; }
    void ClearSelected() { d_selected = Entity(); }

    bool IsGameRunning() const { return d_playingGame; }

    Camera& GetEditorCamera() { return d_editorCamera; }
    Window* GetWindow() { return d_core.window; }

    std::shared_ptr<Scene> GetScene() { return d_activeScene; }
};

}