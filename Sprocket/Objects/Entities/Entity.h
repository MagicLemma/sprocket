#pragma once
#include "Components.h"

#include <vector>
#include <memory>
#include <array>
#include <utility>

namespace Sprocket {

constexpr std::size_t MAX_COMPONENTS = 64;

class Entity;
using EntityManager = std::vector<Entity>;

class Entity
{
    std::vector<std::shared_ptr<Component>> d_components;  // Iteration and ownership
    std::array<Component*, MAX_COMPONENTS> d_componentPtrs;  // Index lookup

public:
    Entity();

    template <typename T>
    T* addComponent();

    template <typename T>
    bool hasComponent() const;

    template <typename T>
    T& getComponent() const;
};

template <typename T>
T* Entity::addComponent()
{
    auto component = std::make_shared<T>();
    d_components.emplace_back(component);
    d_componentPtrs[getComponentTypeId<T>()] = component.get();
    return component.get();
}

template <typename T> bool Entity::hasComponent() const
{
    return d_componentPtrs[getComponentTypeId<T>()] != nullptr;
}

template <typename T> T& Entity::getComponent() const
{
    auto component = d_componentPtrs[getComponentTypeId<T>()];
    return *static_cast<T*>(component);
}

}