#include "GameGrid.h"
#include "CameraUtils.h"

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
    , d_hoveredSquare(std::make_shared<Entity>())
    , d_selectedSquare(std::make_shared<Entity>())
    , d_hovered({0.0, 0.0})
    , d_selected({})
{
    auto gridSqare = GetHoveredSquare();

    d_hoveredSquare->Name() = "Hovered Grid Highlighter";
    auto model1 = d_hoveredSquare->Add<ModelComponent>();
    model1->model = gridSqare;
    model1->material.texture = Texture::White();
    model1->material.reflectivity = 0.0f;
    model1->scale = 0.3f;
    d_entityManager->AddEntity(d_hoveredSquare);

    d_selectedSquare->Name() = "Selected Grid Highlighter";
    auto model2 = d_selectedSquare->Add<ModelComponent>();
    model2->model = gridSqare;
    model2->material.texture = Texture::White();
    model2->material.reflectivity = 0.0f;
    model2->scale = 0.5f;
    d_entityManager->AddEntity(d_selectedSquare);

    d_modelManager->LoadModel("GG_Cube", "Resources/Models/BetterTree.obj");

    d_keyboard.ConsumeAll(false);
}

void GameGrid::OnUpdate(Window* window, Entity* camera)
{
    d_mouse.OnUpdate();
    Maths::vec3 cameraPos = camera->Position();
    Maths::vec3 direction = Maths::GetMouseRay(
        d_mouse.GetMousePos(),
        window->Width(),
        window->Height(),
        CameraUtils::MakeView(*camera),
        CameraUtils::MakeProj(*camera)
    );

    float lambda = -cameraPos.y / direction.y;
    Maths::vec3 mousePos = cameraPos + lambda * direction;
    d_hovered = {(int)std::floor(mousePos.x), (int)std::floor(mousePos.z)};

    d_hoveredSquare->Position() = { d_hovered.x + 0.5f, 0.05f, d_hovered.y + 0.5f };
    if (d_selected.has_value()) {
        d_selectedSquare->Position() = { d_selected.value().x + 0.5f, 0.05f, d_selected.value().y + 0.5f };
    } else {
        d_selectedSquare->Position() = { 0.5f, -1.0f, 0.5f };
    }
}

void GameGrid::OnEvent(Event& event)
{
    if (event.IsConsumed()) { return; }

    static std::uniform_real_distribution d(0.0f, 360.0f);
    static std::uniform_real_distribution ef(1.0f, 1.3f);
    static std::mt19937 gen;

    d_mouse.OnEvent(event);
    d_keyboard.OnEvent(event);

    if (auto e = event.As<MouseButtonPressedEvent>()) {
        d_selected = d_hovered;
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
        d_gridEntities[{x, z}] = entity;
    }
}

void GameGrid::RemoveEntity(int x, int z)
{
    auto it = d_gridEntities.find({x, z});
    if (it == d_gridEntities.end()) {
        SPKT_LOG_WARN("No entity exists at this coord!");
    }
    else {
        it->second->Remove<GridComponent>();
        d_gridEntities.erase(it);
    }
}

Sprocket::Entity* GameGrid::At(int x, int z) const
{
    auto it = d_gridEntities.find({x, z});
    if (it != d_gridEntities.end()) {
        return it->second;
    }
    return nullptr;
}

Sprocket::Entity* GameGrid::Hovered() const
{
    return At(d_hovered.x, d_hovered.y);
}

Sprocket::Entity* GameGrid::Selected() const
{
    if (d_selected.has_value()) {
        return At(d_selected.value().x, d_selected.value().y);
    }
    return nullptr;
}

void GameGrid::DeleteSelected()
{
    auto selected = Selected();
    if (selected != nullptr) {
        selected->Kill();
        auto pos = SelectedPosition().value();
        RemoveEntity(pos.x, pos.y);
    }
}