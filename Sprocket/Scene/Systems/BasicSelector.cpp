#include "BasicSelector.h"
#include "Scene.h"
#include "Components.h"
#include "Log.h"

namespace Sprocket {

BasicSelector::BasicSelector()
    : d_selectedEntity()
{
}

void BasicSelector::OnStartup(Scene& scene)
{
    scene.GetRegistry()->OnRemove<SelectComponent>([&](ECS::Entity entity) {
        if (entity == d_selectedEntity) { SetSelected(ECS::Null); }
    });
}

void BasicSelector::SetSelected(ECS::Entity entity)
{
    // First clear the previous
    if (d_selectedEntity.Valid()) {
        d_selectedEntity.Get<SelectComponent>().selected = false;
        d_selectedEntity = ECS::Null;
    }

    // Now set the new one if the given entity is valid
    if (entity.Valid()) {
        if (!entity.Has<SelectComponent>()) {
            SPKT_LOG_WARN("Attempted to select entity with no SelectComponent");
        }
        else {
            entity.Get<SelectComponent>().selected = true;
            d_selectedEntity = entity;
        }
    }
}

}