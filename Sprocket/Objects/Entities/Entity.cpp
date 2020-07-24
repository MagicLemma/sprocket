#include "Entity.h"

namespace Sprocket {

Entity::Entity()
    : d_registry(nullptr)
    , d_entity(entt::null)
{}


Entity::Entity(entt::registry* registry, entt::entity entity)
    : d_registry(registry)
    , d_entity(entity)
{}

bool operator==(const Entity& lhs, const Entity& rhs)
{
    return lhs.Id() == rhs.Id();
}

}