#pragma once
#include "Event.h"

#include <array>

namespace Sprocket {

inline std::size_t counter()
{
    static std::size_t id = 0;
    return ++id;
}

template <typename T>
inline std::size_t getComponentTypeId() noexcept
{
    static std::size_t id = counter();
    return id;
}

class Entity;

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