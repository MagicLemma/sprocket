#pragma once
#include <Sprocket.h>

#include <memory>
#include <unordered_map>
#include <optional>

struct GridComponent
{
    int x;
    int z;
};

class GameGrid : public Sprocket::EntitySystem
{
public:
    using GridMap = std::unordered_map<
        std::pair<int, int>,
        Sprocket::Entity,
        Sprocket::HashPair
    >;

private:
    Sprocket::Window*       d_window;
    Sprocket::KeyboardProxy d_keyboard;
    Sprocket::MouseProxy    d_mouse;

    Sprocket::Entity d_camera;

    Sprocket::Entity d_hoveredSquare;
    Sprocket::Maths::ivec2 d_hovered;

    Sprocket::Entity d_selectedSquare;
    std::optional<Sprocket::Maths::ivec2> d_selected;

    std::size_t Key(int x, int z);
    GridMap d_gridEntities; 

public:
    GameGrid(Sprocket::Window* window);

    void OnStartup(Sprocket::EntityManager& manager) override;
    void OnUpdate(Sprocket::EntityManager& manager, double dt) override;
    void OnEvent(Sprocket::Event& event) override;

    Sprocket::Entity At(int x, int z) const;

    void AddEntity(Sprocket::Entity* entity, int x, int z);
    void RemoveEntity(int x, int z);

    void SetCamera(Sprocket::Entity entity) { d_camera = entity; }

    Sprocket::Entity Hovered() const;
    Sprocket::Entity Selected() const;

    std::optional<Sprocket::Maths::ivec2> SelectedPosition() const { return d_selected; }

    void DeleteSelected();
};