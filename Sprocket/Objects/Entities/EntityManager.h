#pragma once
#include "Entity.h"
#include "Event.h"

#include <vector>
#include <memory>

namespace Sprocket {

class EntityManager
{
    std::vector<std::shared_ptr<Entity>> d_entities;

public:
    std::shared_ptr<Entity> addEntity();

    void update();
    bool handleEvent(const Event& event);

    std::vector<std::shared_ptr<Entity>> entities();
};

}