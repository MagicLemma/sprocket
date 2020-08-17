#pragma once
#include "EntitySystem.h"
#include "HashPair.h"
#include "Window.h"
#include "KeyboardProxy.h"
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
    KeyboardProxy d_keyboard;
    MouseProxy    d_mouse;

    Entity d_camera;

    Entity d_hoveredSquare;
    Maths::ivec2 d_hovered;

    Entity d_selectedSquare;
    std::optional<Maths::ivec2> d_selected;

    std::size_t Key(int x, int z);
    GridMap d_gridEntities; 

public:
    GameGrid(Window* window);

    void OnStartup(EntityManager& manager) override;
    void OnUpdate(EntityManager& manager, double dt) override;
    void OnEvent(Event& event) override;

    Entity At(int x, int z) const;

    void AddEntity(Entity* entity, int x, int z);
    void RemoveEntity(int x, int z);

    void SetCamera(Entity entity) { d_camera = entity; }

    Entity Hovered() const;
    Entity Selected() const;

    std::optional<Maths::ivec2> SelectedPosition() const { return d_selected; }

    void DeleteSelected();
};

}