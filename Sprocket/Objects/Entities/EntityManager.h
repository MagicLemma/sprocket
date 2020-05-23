#pragma once
#include "Entity.h"
#include "EntitySystem.h"
#include "EntityRenderer.h"
#include "Event.h"

#include <vector>
#include <map>

namespace Sprocket {
    
class EntityManager
{
public:
    using Entities = std::map<std::size_t, std::shared_ptr<Entity>>;

private:
    const std::vector<EntitySystem*> d_systems;
        // All systems must be given at creation.

    Entities d_entities;

public:
    EntityManager(const std::vector<EntitySystem*> systems);

    void addEntity(std::shared_ptr<Entity> entity);
    void update(float dt);
    void handleEvent(Event& event);

    void draw(EntityRenderer* renderer);

    const Entities& entities() const { return d_entities; }
};

}