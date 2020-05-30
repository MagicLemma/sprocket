#pragma once
#include "EntitySystem.h"
#include "Window.h"
#include "Camera.h"
#include "Lens.h"
#include "PhysicsEngine.h"
#include "MouseProxy.h"

namespace Sprocket {
    
class Selector : public EntitySystem
{
    Window* d_window;
    Camera* d_camera;
    Lens* d_lens;

    PhysicsEngine* d_physicsEngine;

    bool d_enabled;

    Entity* d_hoveredEntity;
    Entity* d_selectedEntity;
        // Do not edit these directly, clear and set them with the functions
        // below.

    MouseProxy d_mouse;

    void setHovered(Entity* entity);

    void clearHovered();
    void clearSelected();
        // Sets the corresponding pointer to nullptr. If the previous value
        // was not null, update their internal state before setting the
        // pointer to nullptr.

    Entity* getMousedOver();
        // Returns a pointer to the entity that the mouse is currently
        // over, and a nullptr if there isn't one.

public:
    Selector(
        Window* window,
        Camera* camera,
        Lens* lens,
        PhysicsEngine* physicsEngine
    );
    ~Selector() {}

    void updateSystem(float dt) override;

    void handleEvent(Event& event) override;

    void deregisterEntity(const Entity& entity) override;

    void enable(bool newEnabled);

    void setSelected(Entity* entity);
        // Sets the current selected Entity. If called with a nullptr,
        // it will clear the currently selected Entity.

    Entity* hoveredEntity() const { return d_hoveredEntity; }
    Entity* selectedEntity() const { return d_selectedEntity; }
};

}