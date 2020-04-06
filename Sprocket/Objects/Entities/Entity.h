#pragma once
#include "Model3D.h"
#include "Material.h"
#include "Maths.h"
#include "Model3DLoader.h"
#include "PositionComponent.h"
#include "ModelComponent.h"
#include "Component.h"

#include <vector>
#include <memory>

namespace Sprocket {

class Entity
{
    // Iteration data structures
    std::vector<std::shared_ptr<Component>> d_components;
        // Components stored contiguously for internal iteration. 

    // Index lookup data structures
    ComponentArray  d_componentPtrs;
        // Pointers to the Components with indices matching the bitset.

public:
    Entity();

    void update();
    bool handleEvent(const Event& event);

    template <typename T> bool hasComponent() const
    {
        return d_componentPtrs[getComponentTypeId<T>()] != nullptr;
    }

    template <typename T, typename... Args>
    std::shared_ptr<T> addComponent(Args&&... args)
    {
        auto component = std::make_shared<T>(std::forward<Args>(args)...);
        component->setOwner(this);
        d_components.emplace_back(component);
        d_componentPtrs[getComponentTypeId<T>()] = component.get();
        return component;
    }

    template <typename T> T& getComponent() const
    {
        auto component = d_componentPtrs[getComponentTypeId<T>()];
        return *static_cast<T*>(component);
    }
};

}