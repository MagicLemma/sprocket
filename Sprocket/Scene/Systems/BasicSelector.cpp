#include "BasicSelector.h"
#include "Scene.h"
#include "Components.h"
#include "Log.h"

namespace Sprocket {

void BasicSelector::Set(ECS::Entity entity)
{
    d_selected = entity;
}

ECS::Entity BasicSelector::Get() const
{
    return d_selected.Valid() ? d_selected : ECS::Null;
}

}