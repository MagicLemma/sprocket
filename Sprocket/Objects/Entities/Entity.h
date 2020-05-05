#pragma once
#include "Components.h"

#include <vector>
#include <memory>
#include <array>
#include <utility>

namespace Sprocket {

constexpr std::size_t MAX_COMPONENTS = 64;

class Entity
{
    std::array<std::shared_ptr<Component>, MAX_COMPONENTS> d_components;

    const std::size_t d_id;

public:
    Entity();

    template <typename T>
    T* add();

    template <typename T>
    bool has() const;

    template <typename T>
    T& get() const;

    std::size_t id() const { return d_id; }

};

template <typename T>
T* Entity::add()
{
    auto component = std::make_shared<T>();
    d_components[getComponentTypeId<T>()] = component;
    return component.get();
}

template <typename T> bool Entity::has() const
{
    return d_components[getComponentTypeId<T>()] != nullptr;
}

template <typename T> T& Entity::get() const
{
    auto component = d_components[getComponentTypeId<T>()];
    return *static_cast<T*>(component.get());
}

}