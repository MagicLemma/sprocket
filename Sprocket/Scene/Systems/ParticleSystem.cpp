#include "ParticleSystem.h"
#include "Components.h"

namespace Sprocket {

ParticleSystem::ParticleSystem(ParticleManager* manager)
    : d_manager(manager)
{
}

void ParticleSystem::OnUpdate(Scene& scene, double dt)
{
    scene.Each<TransformComponent, ParticleComponent>([&](Entity& entity) {
        auto& tc = entity.Get<TransformComponent>();
        auto& pc = entity.Get<ParticleComponent>();

        pc.sinceLastEmit += dt;
        while (pc.sinceLastEmit > pc.interval) {
            Particle p;
            p.position = tc.position;
            p.velocity = pc.velocity;
            p.acceleration = pc.acceleration;
            p.life = pc.life;
            p.scale = pc.scale;
            d_manager->Emit(p);

            pc.sinceLastEmit -= pc.interval;
        }
    });
}

}