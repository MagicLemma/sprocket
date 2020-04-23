#pragma once
#include "Maths.h"
#include "Entity.h"
#include "EntitySystem.h"

#include <unordered_map>
#include <utility>
#include <memory>
#include <variant>

namespace Sprocket {

struct PhysicsEngineImpl;

class PhysicsEngine : public EntitySystem
{
    std::shared_ptr<PhysicsEngineImpl> d_impl;

    const float d_timeStep;
    bool        d_running = false;

public:
    PhysicsEngine(const Maths::vec3& gravity);
    ~PhysicsEngine() {}

    void updateSystem(float dt) override;
    void updateEntity(Entity& entity) override;

    void registerEntity(const Entity& entity) override;
    void deregisterEntity(const Entity& entity) override;

    void running(bool isRunning);
    bool running() const { return d_running; }
};

}