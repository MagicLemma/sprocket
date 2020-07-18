#pragma once
#include <Sprocket.h>

#include <memory>
#include <unordered_map>

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

    std::shared_ptr<Sprocket::Entity> d_highlightSquare;
    int d_x, d_z;

    std::size_t Key(int x, int z);
    GridMap d_gridEntities; 

    void AddEntity(Sprocket::Entity* entity, int x, int z);
    void RemoveEntity(int x, int z);

public:
    GameGrid(Sprocket::EntityManager* entityManager,
             Sprocket::ModelManager*  modelManager);

    void OnUpdate(Sprocket::Window* window,
                  Sprocket::Entity* camera);

    void OnEvent(Sprocket::Event& event);

    Sprocket::Entity* At(int x, int z) const;

    Sprocket::Entity* Selected() const;
};