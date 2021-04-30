#pragma once
#include "EntitySystem.h"
#include "Scene.h"
#include "ParticleManager.h"

namespace Sprocket {

class ParticleSystem : public EntitySystem
{
    ParticleManager* d_manager;

public:
    ParticleSystem(ParticleManager* manager);
    
    void on_update(spkt::registry& registry, const ev::Dispatcher& dispatcher, double dt) override;
};

}