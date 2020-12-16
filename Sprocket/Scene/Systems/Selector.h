#pragma once
#include "EntitySystem.h"
#include "Window.h"
#include "ECS.h"
#include "PhysicsEngine.h"

namespace Sprocket {
    
class Selector : public EntitySystem
{
    Window* d_window;
    ECS::Entity d_camera;

    PhysicsEngine* d_physicsEngine;

    bool d_enabled;

    ECS::Entity d_hoveredEntity;
    ECS::Entity d_selectedEntity;
        // Do not edit these directly, clear and set them with the functions
        // below.

    void SetHovered(ECS::Entity entity);

    void ClearHovered();
    void ClearSelected();
        // Sets the corresponding pointer to nullptr. If the previous value
        // was not null, update their internal state before setting the
        // pointer to nullptr.

    ECS::Entity GetMousedOver();
        // Returns a pointer to the entity that the mouse is currently
        // over, and a nullptr if there isn't one.

public:
    Selector(Window* window, PhysicsEngine* physicsEngine);
    ~Selector() {}

    void OnStartup(Scene& scene) override;
    void OnUpdate(Scene& scene, double dt) override;
    void OnEvent(Scene& scene, Event& event) override;

    void Enable(bool newEnabled);

    void SetSelected(ECS::Entity entity);
        // Sets the current selected Entity. If called with a nullptr,
        // it will clear the currently selected Entity.

    void SetCamera(ECS::Entity camera) { d_camera = camera; }

    ECS::Entity HoveredEntity() const { return d_hoveredEntity; }
    ECS::Entity SelectedEntity() const { return d_selectedEntity; }
};

}