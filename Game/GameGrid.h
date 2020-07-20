#pragma once
#include <Sprocket.h>

#include <memory>
#include <unordered_map>
#include <optional>

struct GridComponent : public Sprocket::Component
{
    int x;
    int z;
};

class GameGrid
{
public:
    using GridMap = std::unordered_map<
        std::pair<int, int>,
        Sprocket::Entity*,
        Sprocket::HashPair
    >;

private:
    Sprocket::EntityManager* d_entityManager;
    Sprocket::ModelManager*  d_modelManager;

    Sprocket::KeyboardProxy d_keyboard;
    Sprocket::MouseProxy    d_mouse;

    std::shared_ptr<Sprocket::Entity> d_hoveredSquare;
    Sprocket::Maths::ivec2 d_hovered;

    std::shared_ptr<Sprocket::Entity> d_selectedSquare;
    std::optional<Sprocket::Maths::ivec2> d_selected;

    std::size_t Key(int x, int z);
    GridMap d_gridEntities; 


public:
    GameGrid(Sprocket::EntityManager* entityManager,
             Sprocket::ModelManager*  modelManager);

    void OnUpdate(Sprocket::Window* window,
                  Sprocket::Entity* camera);

    void OnEvent(Sprocket::Event& event);

    Sprocket::Entity* At(int x, int z) const;

    void AddEntity(Sprocket::Entity* entity, int x, int z);
    void RemoveEntity(int x, int z);

    Sprocket::Entity* Hovered() const;
    Sprocket::Entity* Selected() const;

    std::optional<Sprocket::Maths::ivec2> SelectedPosition() const { return d_selected; }

    void DeleteSelected();
};