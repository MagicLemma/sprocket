#include "ParticleManager.h"
#include "RenderContext.h"
#include "Components.h"
#include "Random.h"
#include "Camera.h"
#include "Scene.h"

namespace spkt {
namespace {

std::unique_ptr<Buffer> GetInstanceBuffer()
{
    BufferLayout layout(sizeof(InstanceData), 5);
    layout.AddAttribute(DataType::FLOAT, 3, DataRate::INSTANCE);
    layout.AddAttribute(DataType::FLOAT, 4, DataRate::INSTANCE);
    layout.AddAttribute(DataType::FLOAT, 3, DataRate::INSTANCE);
    assert(layout.Validate());

    return std::make_unique<Buffer>(layout, BufferUsage::DYNAMIC);
}

}

ParticleManager::ParticleManager()
    : d_instances(GetInstanceBuffer())
{
}

void ParticleManager::on_update(apx::registry& registry, double dt)
{
    auto& ps = get_singleton<ParticleSingleton>(registry);

    std::vector<InstanceData> instance_data(NUM_PARTICLES);
    for (const auto& particle : *ps.particles) {
        if (particle.life > 0.0) {
            instance_data.push_back({
                particle.position,
                {0.0, 0.0, 0.0, 1.0},
                particle.scale
            });
        }
    }
    d_instances->SetData(instance_data);
}

}