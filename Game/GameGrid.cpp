#include "GameGrid.h"

#include <random>

using namespace Sprocket;

namespace {

Model3D GetHoveredSquare()
{
    Vertex3DBuffer vb = {
        {{-1, 0, 1},  {0, 1, 0}, {0, 0}}, // Top left
        {{1, 0, 1},   {0, 1, 0}, {1, 0}}, // Top right
        {{-1, 0, -1}, {0, 1, 0}, {0, 1}}, // Bottom left
        {{1, 0, -1},  {0, 1, 0}, {1, 1}}  // Bottom right
    };
    IndexBuffer ib = { 0, 1, 2, 3, 2, 1 };
    return Model3D(vb, ib);
}

}

GameGrid::GameGrid(EntityManager* entityManager,
                   ModelManager*  modelManager)
    : d_entityManager(entityManager)
    , d_modelManager(modelManager)
    , d_highlightSquare(std::make_shared<Entity>())
{
    d_highlightSquare->Name() = "Grid Highlighter";
    auto model = d_highlightSquare->Add<ModelComponent>();
    model->model = GetHoveredSquare();
    model->material.texture = Texture::White();
    model->material.reflectivity = 0.0f;
    model->scale = 0.5f;
    d_entityManager->AddEntity(d_highlightSquare);

    d_modelManager->LoadModel("GG_Cube", "Resources/Models/Cube.obj");

    d_keyboard.ConsumeAll(false);
}

std::size_t GameGrid::Key(int x, int z)
{
    return (std::size_t(x) << 32) | (unsigned int )z;
}

void GameGrid::OnUpdate(Window* window, Camera* camera, Lens* lens)
{
    d_mouse.OnUpdate();
    Maths::vec3 cameraPos = camera->Position();
    Maths::vec3 direction = Maths::GetMouseRay(
        d_mouse.GetMousePos(),
        window->Width(),
        window->Height(),
        camera->View(),
        lens->Projection()
    );

    float lambda = -cameraPos.y / direction.y;
    Maths::vec3 mousePos = cameraPos + lambda * direction;
    d_x = (int)std::floor(mousePos.x);
    d_z = (int)std::floor(mousePos.z);

    d_highlightSquare->Position() = { d_x + 0.5f, 0.05f, d_z + 0.5f };
}

void GameGrid::OnEvent(Event& event)
{
    if (event.IsConsumed()) { return; }

    d_mouse.OnEvent(event);
    d_keyboard.OnEvent(event);

    if (auto e = event.As<MouseButtonPressedEvent>()) {
        auto it = d_gridEntities.find(Key(d_x, d_z));
        if (e->Button() == Mouse::LEFT && it == d_gridEntities.end()) {                SPKT_LOG_INFO("Grid square empty!");
            auto newEntity = std::make_shared<Entity>();
            newEntity->Name() = "New Cube";
            auto modelData = newEntity->Add<ModelComponent>();
            modelData->model = d_modelManager->GetModel("GG_Cube");
            modelData->material.texture = Texture::White();
            modelData->material.shineDamper = 10.0f;
            modelData->material.reflectivity = 0.0f;
            modelData->scale = 0.5f;
            d_entityManager->AddEntity(newEntity);
            AddEntity(newEntity.get(), d_x, d_z);
            e->Consume();
        }
        if (e->Button() == Mouse::RIGHT && it != d_gridEntities.end()) {
            it->second->Kill();
            RemoveEntity(d_x, d_z);
            e->Consume();
        }
    }
}

void GameGrid::AddEntity(Sprocket::Entity* entity, int x, int z)
{
    if (entity->Has<GridComponent>()) {
        SPKT_LOG_WARN("Entity already in grid!");
    }
    else {
        auto c = entity->Add<GridComponent>();  // Mark it as in the grid.
        c->x = x;
        c->z = z;
        entity->Position() = {x + 0.5f, 0.05f, z + 0.5f};
        d_gridEntities[Key(x, z)] = entity;
    }
}

void GameGrid::RemoveEntity(int x, int z)
{
    auto it = d_gridEntities.find(Key(x, z));
    if (it == d_gridEntities.end()) {
        SPKT_LOG_WARN("No entity exists at this coord!");
    }
    else {
        it->second->Remove<GridComponent>();
        d_gridEntities.erase(it);
    }
}