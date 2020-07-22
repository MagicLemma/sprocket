#pragma once
#include "Components.h"

#include <vector>
#include <memory>
#include <array>
#include <utility>

#include <entt/entt.hpp>

namespace Sprocket {

constexpr std::size_t MAX_COMPONENTS = 64;

class Entity
{
    entt::registry* d_registry;
    entt::entity    d_entity;

    std::array<std::shared_ptr<Component>, MAX_COMPONENTS> d_components;
    
public:
    Entity();
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
    auto component = std::make_shared<T>();
    d_components[GetComponentTypeId<T>()] = component;
    return component.get();
}

template <typename T> bool Entity::Has() const
{
    return d_components[GetComponentTypeId<T>()] != nullptr;
}

template <typename T> T& Entity::Get() const
{
    auto component = d_components[GetComponentTypeId<T>()];
    return *static_cast<T*>(component.get());
}

template <typename T> void Entity::Remove()
{
    d_components[GetComponentTypeId<T>()] = nullptr;
}

void Kill(Entity& e);
bool Alive(Entity& e);

}