#pragma once
#include "Maths.h"
#include "Model3D.h"
#include "Material.h"

#include <cstddef>
#include <vector>

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

struct Component {
    virtual ~Component() {}
};

struct PositionComponent : public Component
{
    Maths::vec3 position;
    Maths::vec3 rotation;
    float       scale;
};

struct ModelComponent : public Component
{
    Model3D               model;
    std::vector<Material> materials;
};

struct PhysicsComponent : public Component
{
    Maths::vec3 velocity;
    Maths::vec3 acceleration;
};

struct PlayerComponent : public Component
{
};


}