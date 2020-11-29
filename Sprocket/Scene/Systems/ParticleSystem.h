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
    
    void OnUpdate(Scene& scene, double dt, bool active) override;
};

}