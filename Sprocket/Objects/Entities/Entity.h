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
    Entity(entt::registry* registry, entt::entity entity);

    entt::entity Id() const { return d_entity; }

    template <typename T> T* Add();
    template <typename T> bool Has() const;
    template <typename T> T& Get() const;
    template <typename T> void Remove();
};

template <typename T>
T* Entity::Add()
{
    d_registry->assign<T>(d_entity);
    return &d_registry->get<T>(d_entity);
}

template <typename T> bool Entity::Has() const
{
    return d_registry->has<T>(d_entity);
}

template <typename T> T& Entity::Get() const
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