#include "EntitySystem.h"
#include "EntityManager.h"

namespace Sprocket {

void EntitySystem::OnUpdate(EntityManager& manager, double dt) {
    OnUpdate(manager.Entities(), dt);
};
    
}