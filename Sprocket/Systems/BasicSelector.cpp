#include "BasicSelector.h"

namespace Sprocket {

BasicSelector::BasicSelector()
    : d_selectedEntity(nullptr)
{
}

void BasicSelector::DeregisterEntity(const Entity& entity)
{
    if (&entity == d_selectedEntity) { SetSelected(nullptr); }
}

void BasicSelector::SetSelected(Entity* entity)
{
    // First clear the previous
    if (d_selectedEntity != nullptr) {
        d_selectedEntity->Get<SelectComponent>().selected = false;
        d_selectedEntity = nullptr;
    }

    // Now set the new one if the given entity is valid
    if (entity != nullptr) {
        if (!entity->Has<SelectComponent>()) {
            SPKT_LOG_WARN("Attempted to select entity with no SelectComponent");
        }
        else {
            entity->Get<SelectComponent>().selected = true;
            d_selectedEntity = entity;
        }
    }
}

}