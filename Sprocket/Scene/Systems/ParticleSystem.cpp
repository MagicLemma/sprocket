#include "ParticleSystem.h"
#include "Components.h"
#include "apecs.hpp"
#include "Random.h"
#include "Scene.h"

namespace spkt {
namespace {

void emit_particle(ParticleSingleton& ps, const particle& p)
{
    (*ps.particles)[ps.next_slot] = p;
    ps.next_slot = --ps.next_slot % NUM_PARTICLES;
}

ParticleSingleton& get_particle_runtime(apx::registry& registry)
{
    auto entity = registry.find<ParticleSingleton>();
    if (!registry.valid(entity)) [[unlikely]] {
        entity = registry.create();
        registry.emplace<Runtime>(entity);
        registry.emplace<NameComponent>(entity, "::ParticleRuntimeSingleton");
        auto& ps = registry.emplace<ParticleSingleton>(entity);
        ps.particles = std::make_shared<std::array<spkt::particle, NUM_PARTICLES>>();
    }
    return registry.get<ParticleSingleton>(entity);
}

}

void particle_system(apx::registry& registry, double dt)
{
    auto& ps = get_particle_runtime(registry);

    for (auto& particle : *ps.particles) {
        particle.life -= dt;
        particle.position += particle.velocity * (float)dt;
        particle.velocity += particle.acceleration * (float)dt;
    }

    for (auto entity : registry.view<ParticleComponent>()) {
        auto& tc = registry.get<Transform3DComponent>(entity);
        auto& pc = registry.get<ParticleComponent>(entity);

        pc.accumulator += (float)dt;
        while (pc.accumulator > pc.interval) {
            float n = pc.velocityNoise;

            float r = Random<float>(0, pc.velocityNoise);
            float phi = Random<float>(0, 3.142f);
            float theta = Random<float>(0, 6.284f);
            glm::vec3 noise = {r * std::sin(theta) * std::cos(phi), r * std::sin(theta) * std::sin(phi), r * std::cos(theta)};

            spkt::particle p2;
            p2.position = tc.position;
            p2.velocity = pc.velocity + noise;
            p2.acceleration = pc.acceleration;
            p2.life = pc.life;
            p2.scale = pc.scale;
            emit_particle(ps, p2);

            pc.accumulator -= pc.interval;
        }
    }
}

}