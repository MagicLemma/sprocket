#include "ParticleManager.h"
#include "ModelManager.h"
#include "RenderContext.h"
#include "Components.h"
#include "Random.h"
#include "CameraUtils.h"

namespace Sprocket {
namespace {

std::shared_ptr<Buffer> GetInstanceBuffer()
{
    BufferLayout layout(sizeof(InstanceData), 5);
    layout.AddAttribute(DataType::FLOAT, 3, DataRate::INSTANCE);
    layout.AddAttribute(DataType::FLOAT, 4, DataRate::INSTANCE);
    layout.AddAttribute(DataType::FLOAT, 3, DataRate::INSTANCE);
    assert(layout.Validate());

    return std::make_shared<Buffer>(layout, BufferUsage::DYNAMIC);
}

}

ParticleManager::ParticleManager()
    : d_instances(GetInstanceBuffer())
{
}

void ParticleManager::Emit(const Particle& particle)
{
    auto& p = d_particles[d_index];
    p.position = particle.position;
    p.velocity = particle.velocity;
    p.acceleration = particle.acceleration;
    p.scale = particle.scale;
    p.life = particle.life;
    d_index = --d_index % NUM_PARTICLES;
}

void ParticleManager::OnUpdate(double dt)
{
    d_instanceData.clear();
    for (auto& particle : d_particles) {
        particle.life -= dt;
        particle.position += particle.velocity * (float)dt;
        particle.velocity += particle.acceleration * (float)dt;
        float r = Random<float>(1.0f, 3.0f);
        if (particle.life > 0.0) {
            d_instanceData.push_back({
                particle.position,
                {0.0, 0.0, 0.0, 1.0},
                particle.scale
            });
        }
    }
    d_instances->SetData(d_instanceData);
}

}