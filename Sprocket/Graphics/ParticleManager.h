#pragma once
#include "Maths.h"
#include "Buffer.h"

#include <memory>
#include <array>
#include <vector>

namespace Sprocket {

struct Particle
{
    glm::vec3 position      = {0.0, 0.0, 0.0};
    glm::vec3 velocity      = {0.0, 0.0, 0.0};
    glm::vec3 acceleration  = {0.0, 0.0, 0.0};
    glm::vec3 scale         = {1.0, 1.0, 1.0};

    double life = -1.0;
};

class ParticleManager
{
    static constexpr int NUM_PARTICLES = 10000;

    std::array<Particle, NUM_PARTICLES> d_particles;

    // The position of the next available space to place a particle. This
    // updated whenever emit is called.
    std::size_t d_index = NUM_PARTICLES - 1;

    std::shared_ptr<Buffer> d_instances;

    std::vector<InstanceData> d_instanceData;

public:
    ParticleManager();

    void Emit(const Particle& particle);

    void OnUpdate(double dt);

    std::shared_ptr<Buffer> GetInstances() const { return d_instances; }

};

}