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

class GameGrid : public EntitySystem
{
public:
    using GridMap = std::unordered_map<glm::ivec2, spkt::entity>;

private:
    spkt::entity d_camera;
    spkt::entity d_hoveredSquare;
    spkt::entity d_selectedSquare;

    glm::ivec2 d_hovered; // TODO: make optional
    std::optional<glm::ivec2> d_selected;

    GridMap d_gridEntities; 

public:
    GameGrid();

    void on_startup(spkt::registry& registry) override;
    void on_event(spkt::registry& registry, ev::Event& event) override;
    void on_update(spkt::registry& registry, double dt) override;

    spkt::entity At(spkt::registry& registry, const glm::ivec2& pos) const;

    void SetCamera(spkt::entity entity) { d_camera = entity; }

    spkt::entity Hovered(spkt::registry& registry) const;
    spkt::entity Selected(spkt::registry& registry) const;

    std::optional<glm::ivec2> SelectedPosition() const { return d_selected; }
};

}