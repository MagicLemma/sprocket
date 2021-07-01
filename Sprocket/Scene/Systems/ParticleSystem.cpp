#include "ParticleSystem.h"
#include "Components.h"
#include "ECS.h"
#include "Random.h"
#include "Scene.h"
#include "ParticleManager.h"

namespace spkt {

void particle_system_init(spkt::registry& registry, ParticleManager* particle_manager)
{
    auto singleton = registry.find<Singleton>();
    auto& ps = registry.emplace<ParticleSingleton>(singleton);
    ps.particle_manager = particle_manager;
}

void particle_system(spkt::registry& registry, double dt)
{
    auto& ps = get_singleton<ParticleSingleton>(registry);

    for (auto entity : registry.view<ParticleComponent>()) {
        auto& tc = registry.get<Transform3DComponent>(entity);
        auto& pc = registry.get<ParticleComponent>(entity);

        pc.accumulator += dt;
        while (pc.accumulator > pc.interval) {
            float n = pc.velocityNoise;

            float r = Random<float>(0, pc.velocityNoise);
            float phi = Random<float>(0, 3.142);
            float theta = Random<float>(0, 6.284);
            glm::vec3 noise = {r * std::sin(theta) * std::cos(phi), r * std::sin(theta) * std::sin(phi), r * std::cos(theta)};
            
            Particle p;
            p.position = tc.position;
            p.velocity = pc.velocity + noise;
            p.acceleration = pc.acceleration;
            p.life = pc.life;
            p.scale = pc.scale;
            ps.particle_manager->Emit(p);

            pc.accumulator -= pc.interval;
        }
    }
}

}