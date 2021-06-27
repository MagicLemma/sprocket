#pragma once
#include "EntitySystem.h"
#include "ECS.h"

#include <functional>

namespace Sprocket {

class LambdaSystem : public EntitySystem
{
    using lambda_t = std::function<void(spkt::registry&, double)>;
    lambda_t lambda;

public:
    LambdaSystem(const lambda_t& l) : lambda(l) {}

    void on_update(spkt::registry& registry, double dt) override
    {
        lambda(registry, dt);
    }
};

}