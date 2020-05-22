#pragma once
#include "EntitySystem.h"
#include "Window.h"
#include "Camera.h"
#include "Lens.h"
#include "PhysicsEngine.h"

namespace Sprocket {
    
class Selector : public EntitySystem
{
    Window* d_window;
    Camera* d_camera;
    Lens* d_lens;

    PhysicsEngine* d_physicsEngine;

    bool d_enabled;

    Entity* d_hoveredEntity;;
    Entity* d_selectedEntity;

public:
    Selector(
        Window* window,
        Camera* camera,
        Lens* lens,
        PhysicsEngine* physicsEngine
    );
    ~Selector() {}

    void updateEntity(float dt, Entity& entity) override;
    void updateSystem(float dt) override;

    bool handleEvent(Event& event) override;

    void registerEntity(const Entity& entity) override {}
    void deregisterEntity(const Entity& entity) override;

    void enable(bool newEnabled) { d_enabled = newEnabled; }

    Entity* hoveredEntity() const { return d_hoveredEntity; }
    Entity* selectedEntity() const { return d_selectedEntity; }

    void clear();
};

}