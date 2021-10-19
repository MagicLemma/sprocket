#include "particle_system.h"
#include <Anvil/ecs/ecs.h>

#include <Sprocket/Utility/Random.h>

#include <glm/glm.hpp>

#include <array>
#include <memory>

namespace anvil {
namespace {

anvil::ParticleSingleton& get_particle_runtime(anvil::registry& registry)
{
    auto entity = registry.find<anvil::ParticleSingleton>();
    if (!registry.valid(entity)) [[unlikely]] {
        entity = registry.create();
        registry.emplace<anvil::Runtime>(entity);
        registry.emplace<anvil::NameComponent>(entity, "::ParticleRuntimeSingleton");
        auto& ps = registry.emplace<anvil::ParticleSingleton>(entity);
        ps.particles = std::make_shared<std::array<spkt::particle, spkt::NUM_PARTICLES>>();
    }
    return registry.get<anvil::ParticleSingleton>(entity);
}

// Generates a random point in a sphere of radius R
glm::vec3 random_noise(float R)
{
    const float r = R * spkt::random_uniform();
    const float phi = spkt::random_half_angle();
    const float theta = spkt::random_angle();
    return {
        r * glm::sin(theta) * glm::cos(phi),
        r * glm::sin(theta) * glm::sin(phi),
        r * glm::cos(theta)
    };
}

}

void particle_system(anvil::registry& registry, double dt)
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
    for (auto [pc, tc] : registry.view_get<anvil::ParticleComponent, anvil::Transform3DComponent>()) {
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
            ps.next_slot = --ps.next_slot % spkt::NUM_PARTICLES;

            pc.accumulator -= pc.interval;
        }
    }
}

}