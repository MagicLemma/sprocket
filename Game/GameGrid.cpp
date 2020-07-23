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

GameGrid::GameGrid(EntityManager* entityManager)
    : d_entityManager(entityManager)
    , d_hoveredSquare(entityManager->NewEntity())
    , d_selectedSquare(entityManager->NewEntity())
    , d_hovered({0.0, 0.0})
    , d_selected({})
{
    auto gridSqare = GetHoveredSquare();

    auto& n1 = d_hoveredSquare.Add<NameComponent>();
    n1.name = "Hovered Grid Highlighter";
    auto& tr1 = d_hoveredSquare.Add<TransformComponent>();
    auto& model1 = d_hoveredSquare.Add<ModelComponent>();
    model1.model = gridSqare;
    model1.material.texture = Texture::White();
    model1.material.reflectivity = 0.0f;
    model1.scale = 0.3f;
    d_entityManager->AddEntity(d_hoveredSquare);

    auto& n2 = d_selectedSquare.Add<NameComponent>();
    n2.name = "Selected Grid Highlighter";
    auto& tr2 = d_selectedSquare.Add<TransformComponent>();
    auto& model2 = d_selectedSquare.Add<ModelComponent>();
    model2.model = gridSqare;
    model2.material.texture = Texture::White();
    model2.material.reflectivity = 0.0f;
    model2.scale = 0.5f;
    d_entityManager->AddEntity(d_selectedSquare);

    d_keyboard.ConsumeAll(false);
}

void GameGrid::OnUpdate(Window* window, Entity* camera)
{
    auto& camTr = camera->Get<TransformComponent>();

    d_mouse.OnUpdate();
    Maths::vec3 cameraPos = camTr.position;
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

    d_hoveredSquare.Get<TransformComponent>().position = { d_hovered.x + 0.5f, 0.05f, d_hovered.y + 0.5f };
    if (d_selected.has_value()) {
        d_selectedSquare.Get<TransformComponent>().position = { d_selected.value().x + 0.5f, 0.05f, d_selected.value().y + 0.5f };
    } else {
        d_selectedSquare.Get<TransformComponent>().position = { 0.5f, -1.0f, 0.5f };
    }
}

void GameGrid::OnEvent(Event& event)
{
    if (event.IsConsumed()) { return; }

    d_mouse.OnEvent(event);
    d_keyboard.OnEvent(event);

    if (auto e = event.As<MouseButtonPressedEvent>()) {
        if (e->Button() == Mouse::LEFT) {
            d_selected = d_hovered;
        }
        else {
            d_selected = {};
        }
    }
}

void GameGrid::AddEntity(Sprocket::Entity* entity, int x, int z)
{
    if (!entity->Has<TransformComponent>()) {
        SPKT_LOG_ERROR("Entity cannot go in grid, no transform!");
        return;
    }

    if (entity->Has<GridComponent>()) {
        SPKT_LOG_WARN("Entity already in grid!");
    }
    else {
        auto& c = entity->Add<GridComponent>();  // Mark it as in the grid.
        c.x = x;
        c.z = z;
        entity->Get<TransformComponent>().position.x = x + 0.5f;
        entity->Get<TransformComponent>().position.z = z + 0.5f;
        d_gridEntities[{x, z}] = *entity;
    }
}

void GameGrid::RemoveEntity(int x, int z)
{
    auto it = d_gridEntities.find({x, z});
    if (it == d_gridEntities.end()) {
        SPKT_LOG_WARN("No entity exists at this coord!");
    }
    else {
        it->second.Remove<GridComponent>();
        d_gridEntities.erase(it);
    }
}

Sprocket::Entity GameGrid::At(int x, int z) const
{
    auto it = d_gridEntities.find({x, z});
    if (it != d_gridEntities.end()) {
        return it->second;
    }
    return Sprocket::Entity();
}

Sprocket::Entity GameGrid::Hovered() const
{
    return At(d_hovered.x, d_hovered.y);
}

Sprocket::Entity GameGrid::Selected() const
{
    if (d_selected.has_value()) {
        return At(d_selected.value().x, d_selected.value().y);
    }
    return Sprocket::Entity();
}

void GameGrid::DeleteSelected()
{
    auto selected = Selected();
    if (!selected.Null()) {
        Kill(selected);
        auto pos = SelectedPosition().value();
        RemoveEntity(pos.x, pos.y);
    }
}