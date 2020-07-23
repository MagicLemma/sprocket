#include "EntitySystem.h"

namespace Sprocket {

void EntitySystem::OnUpdate(std::map<entt::entity, Entity>& entities, double dt)
{
    for (auto& [id, entity] : entities) {
        UpdateEntity(dt, entity);
    }

    UpdateSystem(dt);

    for (auto& [id, entity] : entities) {
        PostUpdateEntity(dt, entity);
    }
}
    
}