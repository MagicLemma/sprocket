#pragma once
#include "ECS.h"
#include "EntitySystem.h"
#include "Hashing.h"
#include "Window.h"
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
    using GridMap = std::unordered_map<glm::ivec2, ecs::Entity>;

private:
    Window*       d_window;

    ecs::Entity d_camera;
    ecs::Entity d_hoveredSquare;
    ecs::Entity d_selectedSquare;

    glm::ivec2 d_hovered; // TODO: make optional
    std::optional<glm::ivec2> d_selected;

    GridMap d_gridEntities; 

public:
    GameGrid(Window* window);

    void on_startup(ecs::Registry& registry) override;
    void on_update(ecs::Registry& registry, double dt) override;
    void on_event(ecs::Registry& registry, ev::Event& event) override;

    ecs::Entity At(const glm::ivec2& pos) const;

    void SetCamera(ecs::Entity entity) { d_camera = entity; }

    ecs::Entity Hovered() const;
    ecs::Entity Selected() const;

    std::optional<glm::ivec2> SelectedPosition() const { return d_selected; }
};

}