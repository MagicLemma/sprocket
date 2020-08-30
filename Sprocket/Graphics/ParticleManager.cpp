#include "ParticleManager.h"
#include "ModelManager.h"
#include "RenderContext.h"
#include "Components.h"
#include "Light.h"
#include "Random.h"
#include "CameraUtils.h"

namespace Sprocket {

ParticleManager::ParticleManager()
    : d_model(ModelManager::LoadModel("Resources/Models/Particle.obj"))
    , d_instances(std::make_shared<InstanceBuffer>(NUM_PARTICLES))
{
}

void ParticleManager::Emit(const Particle& particle)
{
    auto& p = d_particles[d_index];
    p.position = particle.position;
    p.velocity = particle.velocity;
    p.scale = particle.scale;
    p.life = particle.life;
    d_index = --d_index % NUM_PARTICLES;
}

void ParticleManager::OnUpdate(double dt)
{
    d_instances->Clear();

    for (auto& particle : d_particles) {
        particle.life -= dt;
        particle.position += particle.velocity * (float)dt;
        particle.velocity += particle.acceleration * (float)dt;

        float r = Random<float>(1.0f, 3.0f);
        if (particle.life > 0.0) {
            d_instances->Add({
                particle.position,
                {0.0, 0.0, 0.0, 1.0},
                particle.scale,
                1.0,
                1.0
            });
        }
    }
}

}