#pragma once
#include "VertexArray.h"
#include "Maths.h"
#include "Model3D.h"
#include "InstanceBuffer.h"
#include "Shader.h"
#include "Scene.h"
#include "Light.h"

#include <memory>
#include <array>

namespace Sprocket {

struct Particle
{
    Maths::vec3 position      = {0.0, 0.0, 0.0};
    Maths::vec3 velocity      = {0.0, 0.0, 0.0};
    Maths::vec3 acceleration  = {0.0, 0.0, 0.0};
    Maths::vec3 scale         = {1.0, 1.0, 1.0};

    double life = -1.0;
};

class ParticleManager
{
    static constexpr int NUM_PARTICLES = 10000;

    std::array<Particle, NUM_PARTICLES> d_particles;
    std::size_t                         d_index = NUM_PARTICLES - 1;

    std::shared_ptr<Model3D>        d_model;
    std::shared_ptr<InstanceBuffer> d_instances;

public:
    ParticleManager();

    void Emit(const Particle& particle);

    void OnUpdate(double dt);

    std::shared_ptr<Model3D> GetModel() const { return d_model; }
    std::shared_ptr<InstanceBuffer> GetInstances() const { return d_instances; }

};

}