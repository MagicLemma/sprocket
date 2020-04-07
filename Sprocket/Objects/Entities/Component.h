#pragma once
#include "Event.h"

#include <array>

namespace Sprocket {

class Entity;
class Component;

using ComponentId = std::size_t;

inline ComponentId idGenerator()
{
    static ComponentId id = 0;
    return ++id;
}

template <typename T>
inline ComponentId getComponentTypeId() noexcept
{
    static ComponentId id = idGenerator();
    return id;
}

class Component
{
protected:
    Entity* d_owner;

public:
    virtual ~Component() {}

    virtual void update() {}
    virtual bool handleEvent(const Event& event) { return false; }

    void setOwner(Entity* owner) { d_owner = owner; }
};

}