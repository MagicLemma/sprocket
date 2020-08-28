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

void Entity::Kill()
{
    if (!Null()) {
        d_registry->destroy(d_entity);
    }
}

bool operator==(Entity&& lhs, Entity&& rhs)
{
    return lhs.Id() == rhs.Id();
}

bool operator==(const Entity& lhs, const Entity& rhs)
{
    return lhs.Id() == rhs.Id();
}

Entity Entity::NewEntity()
{
    auto e = d_registry->create();
    return Entity(d_registry, e);
}

}