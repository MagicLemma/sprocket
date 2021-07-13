#include "particle_system.h"
#include "ecs.h"
#include "ecs.h"
#include "Random.h"
#include "Scene.h"

namespace spkt {
namespace {

ParticleSingleton& get_particle_runtime(spkt::registry& registry)
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

glm::vec3 random_noise(float R)
// Generates a random point in a sphere of radius R
{
    float r = Random<float>(0, R);
    float phi = Random<float>(0, 3.142f);
    float theta = Random<float>(0, 6.284f);
    return {
        r * glm::sin(theta) * glm::cos(phi),
        r * glm::sin(theta) * glm::sin(phi),
        r * glm::cos(theta)
    };
}

}

void particle_system(spkt::registry& registry, double dt)
{
    auto& ps = get_particle_runtime(registry);

    // Update the positions of all existing particles
    for (auto& particle : *ps.particles) {
        particle.life -= dt;
        particle.position += particle.velocity * (float)dt;
        particle.velocity += particle.acceleration * (float)dt;
    }

    // Go through all particle emitter entities and check to see if they need to emit
    // any particles this frame. If they do, add them to the particle array.
    for (auto entity : registry.view<ParticleComponent>()) {
        auto& tc = registry.get<Transform3DComponent>(entity);
        auto& pc = registry.get<ParticleComponent>(entity);

        pc.accumulator += (float)dt;
        while (pc.accumulator > pc.interval) {
            // Add a particle to the particle array
            (*ps.particles)[ps.next_slot] = {
                .position = tc.position,
                .velocity = pc.velocity + random_noise(pc.velocityNoise),
                .acceleration = pc.acceleration,
                .scale = pc.scale,
                .life = pc.life
            };
            ps.next_slot = --ps.next_slot % NUM_PARTICLES;

            pc.accumulator -= pc.interval;
        }
    }
}

}