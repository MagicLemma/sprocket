#pragma once
#include "EntitySystem.h"
#include "Scene.h"
#include "ParticleManager.h"

namespace spkt {

class ParticleSystem : public EntitySystem
{
    ParticleManager* d_manager;

public:
    ParticleSystem(ParticleManager* manager);
    
    void on_update(spkt::registry& registry, double dt) override;
};

}