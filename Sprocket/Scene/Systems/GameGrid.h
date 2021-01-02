#pragma once
#include "ECS.h"
#include "EntitySystem.h"
#include "HashPair.h"
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
    using GridMap = std::unordered_map<glm::ivec2, ECS::Entity>;

private:
    Window*       d_window;

    ECS::Entity d_camera;
    ECS::Entity d_hoveredSquare;
    ECS::Entity d_selectedSquare;

    glm::ivec2 d_hovered; // TODO: make optional
    std::optional<glm::ivec2> d_selected;

    GridMap d_gridEntities; 

public:
    GameGrid(Window* window);

    void OnStartup(Scene& scene) override;
    void OnUpdate(Scene& scene, double dt) override;
    void OnEvent(Scene& scene, Event& event) override;

    ECS::Entity At(const glm::ivec2& pos) const;

    void SetCamera(ECS::Entity entity) { d_camera = entity; }

    ECS::Entity Hovered() const;
    ECS::Entity Selected() const;

    std::optional<glm::ivec2> SelectedPosition() const { return d_selected; }
};

}