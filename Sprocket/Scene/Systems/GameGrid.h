#pragma once
#include "ECS.h"
#include "EntitySystem.h"
#include "Hashing.h"
#include "Maths.h"

#include <memory>
#include <unordered_map>
#include <optional>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace Sprocket {

struct GameGrid : public EntitySystem
{
    void on_startup(spkt::registry& registry) override;
    void on_event(spkt::registry& registry, ev::Event& event) override;
    void on_update(spkt::registry& registry, double dt) override;

    spkt::entity At(spkt::registry& registry, const glm::ivec2& pos) const;

    spkt::entity Hovered(spkt::registry& registry) const;
    spkt::entity Selected(spkt::registry& registry) const;
};

}