#pragma once
#include "Components.h"

#include <vector>
#include <memory>
#include <array>
#include <utility>

#include <entt/entt.hpp>

namespace Sprocket {

class Entity
{
    entt::registry* d_registry;
    entt::entity    d_entity;
    
public:
    Entity(); // Returns a null entity.
    Entity(entt::registry* registry, entt::entity entity);

    uint32_t Id() const { return entt::to_integral(d_entity); }

    template <typename T> T& Add();
    template <typename T> bool Has() const;
    template <typename T> T& Get();
    template <typename T> const T& Get() const;
    template <typename T> void Remove();

    bool Null() const { return d_entity == entt::null; }

    friend class EntityManager;
};

bool operator==(const Entity& lhs, const Entity& rhs);

template <typename T>
T& Entity::Add()
{
    d_registry->assign<T>(d_entity);
    return d_registry->get<T>(d_entity);
}

template <typename T> bool Entity::Has() const
{
    if (d_entity == entt::null || !d_registry->valid(d_entity)) {
        return false;
    }
    return d_registry->has<T>(d_entity);
}

template <typename T> T& Entity::Get()
{
   return d_registry->get<T>(d_entity);
}

template <typename T> const T& Entity::Get() const
{
   return d_registry->get<T>(d_entity);
}

template <typename T> void Entity::Remove()
{
    d_registry->remove_if_exists<T>(d_entity);
}

void Kill(Entity& e);
bool Alive(Entity& e);

}