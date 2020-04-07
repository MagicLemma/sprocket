#include "Entity.h"

namespace Sprocket {

Entity::Entity()
{
    for (std::size_t i = 0; i != MAX_COMPONENTS; ++i) {
        d_componentPtrs[i] = nullptr;
    }
}

void Entity::update()
{
    for (auto& component : d_components) {
        component->update();
    }
}

bool Entity::handleEvent(const Event& event)
{
    for (auto& component : d_components) {
        if (component->handleEvent(event)) {
            return true;
        }
    }
    return false;
}

}