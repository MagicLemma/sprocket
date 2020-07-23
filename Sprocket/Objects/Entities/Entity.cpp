#include "Entity.h"

namespace Sprocket {

namespace {

std::size_t IdGenerator()
{
    static std::size_t id = 0;
    return id++;
}

}

Entity::Entity()
    : d_registry(nullptr)
    , d_entity(entt::null)
{}


Entity::Entity(entt::registry* registry, entt::entity entity)
    : d_registry(registry)
    , d_entity(entity)
{}

void Kill(Entity& e)
{
    if (e.Has<DeletableComponent>()) {
        e.Get<DeletableComponent>().alive = false;
    }
}

bool Alive(Entity& e)
{
    if (e.Has<DeletableComponent>()) {
        return e.Get<DeletableComponent>().alive;
    }
    return true;
}

bool operator==(const Entity& lhs, const Entity& rhs)
{
    return lhs.Id() == rhs.Id();
}

}