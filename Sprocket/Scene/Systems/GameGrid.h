#pragma once
#include "EntitySystem.h"
#include "HashPair.h"
#include "Window.h"
#include "MouseProxy.h"
#include "Maths.h"

#include <memory>
#include <unordered_map>
#include <optional>

namespace Sprocket {

class GameGrid : public EntitySystem
{
public:
    using GridMap = std::unordered_map<
        std::pair<int, int>, Entity, HashPair
    >;

private:
    Window*       d_window;

    Entity d_camera;
    Entity d_hoveredSquare;
    Entity d_selectedSquare;

    glm::ivec2 d_hovered; // TODO: make optional
    std::optional<glm::ivec2> d_selected;

    GridMap d_gridEntities; 

public:
    GameGrid(Window* window);

    void OnStartup(Scene& scene) override;
    void OnUpdate(Scene& scene, double dt) override;
    void OnEvent(Scene& scene, Event& event) override;

    Entity At(int x, int z) const;

    void SetCamera(Entity entity) { d_camera = entity; }

    Entity Hovered() const;
    Entity Selected() const;

    std::optional<glm::ivec2> SelectedPosition() const { return d_selected; }
};

}