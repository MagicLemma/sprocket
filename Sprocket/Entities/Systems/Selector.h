#pragma once
#include "EntitySystem.h"
#include "Window.h"
#include "Entity.h"
#include "PhysicsEngine.h"
#include "MouseProxy.h"

namespace Sprocket {
    
class Selector : public EntitySystem
{
    Window* d_window;
    Entity d_camera;

    PhysicsEngine* d_physicsEngine;

    bool d_enabled;

    Entity d_hoveredEntity;
    Entity d_selectedEntity;
        // Do not edit these directly, clear and set them with the functions
        // below.

    MouseProxy d_mouse;

    void SetHovered(Entity entity);

    void ClearHovered();
    void ClearSelected();
        // Sets the corresponding pointer to nullptr. If the previous value
        // was not null, update their internal state before setting the
        // pointer to nullptr.

    Entity GetMousedOver();
        // Returns a pointer to the entity that the mouse is currently
        // over, and a nullptr if there isn't one.

public:
    Selector(
        Window* window,
        PhysicsEngine* physicsEngine
    );
    ~Selector() {}

    void OnStartup(EntityManager& manager) override;
    void OnUpdate(EntityManager& manager, double dt) override;
    void OnEvent(Event& event) override;

    void Enable(bool newEnabled);

    void SetSelected(Entity entity);
        // Sets the current selected Entity. If called with a nullptr,
        // it will clear the currently selected Entity.

    void SetCamera(Entity camera) { d_camera = camera; }

    Entity HoveredEntity() const { return d_hoveredEntity; }
    Entity SelectedEntity() const { return d_selectedEntity; }
};

}