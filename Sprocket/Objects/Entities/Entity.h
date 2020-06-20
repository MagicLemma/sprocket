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
    std::string       d_name;
    const std::size_t d_id;

    bool              d_alive;

    Maths::vec3 d_position;
    Maths::quat d_orientation;

    std::array<std::shared_ptr<Component>, MAX_COMPONENTS> d_components;
    
public:
    Entity();

    const std::string& Name() const { return d_name; }
    std::string& Name() { return d_name; }

    std::size_t Id() const { return d_id; }

    bool Alive() const { return d_alive; }
    void Kill() { d_alive = false; }

    void SetPosition(const Maths::vec3& pos) { d_position = pos; }

    const Maths::vec3& Position() const { return d_position; }
    Maths::vec3& Position() { return d_position; }

    const Maths::quat& Orientation() const { return d_orientation; }
    Maths::quat& Orientation() { return d_orientation; }

    Maths::mat4 Transform() const;

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

}