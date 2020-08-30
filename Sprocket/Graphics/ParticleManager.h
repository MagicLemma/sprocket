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
    Maths::vec3 position;
    Maths::vec3 velocity;

    double life;

    Particle(const Maths::vec3& p, const Maths::vec3& v, double l);
    Particle();
};

class ParticleManager
{
    static constexpr int NUM_PARTICLES = 1000;

    Shader d_shader;

    std::array<Particle, NUM_PARTICLES> d_particles;
    std::size_t                         d_index = NUM_PARTICLES - 1;

    std::unique_ptr<VertexArray> d_vao;

    std::shared_ptr<Model3D>        d_model;
    std::shared_ptr<InstanceBuffer> d_instances;

public:
    ParticleManager();

    void Emit(const Particle& particle);

    void Draw(double dt, const Entity& camera, const Lights& lights, Scene& scene);
    void Draw(double dt, const Maths::mat4& proj, const Maths::mat4& view, const Lights& lights, Scene& scene);

};

}