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

    const std::string& name() const { return d_name; }
    std::string& name() { return d_name; }

    std::size_t id() const { return d_id; }

    bool alive() const { return d_alive; }
    void kill() { d_alive = false; }

    const Maths::vec3& position() const { return d_position; }
    Maths::vec3& position() { return d_position; }

    const Maths::quat& orientation() const { return d_orientation; }
    Maths::quat& orientation() { return d_orientation; }

    Maths::mat4 transform() const;

    template <typename T> T* add();
    template <typename T> bool has() const;
    template <typename T> T& get() const;
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