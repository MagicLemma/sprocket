#include "GameGrid.h"
#include "Camera.h"
#include "Components.h"
#include "Scene.h"
#include "MouseEvent.h"
#include "MouseCodes.h"
#include "Log.h"

#include <random>
#include <cassert>

namespace Sprocket {

std::string Name(const ecs::Entity& e) {
    if (e.Has<NameComponent>()) {
        return e.Get<NameComponent>().name;
    }
    return "Entity";
}

GameGrid::GameGrid(Window* window)
    : d_window(window)
    , d_hovered({0.0, 0.0})
    , d_selected({})
{
}

void GameGrid::OnStartup(Scene& scene)
{
    std::string gridSquare = "Resources/Models/Square.obj";

    d_hoveredSquare = scene.Entities().New();
    auto& n1 = d_hoveredSquare.Add<NameComponent>();
    d_hoveredSquare.Add<TemporaryComponent>();
    n1.name = "Hovered Grid Highlighter";
    auto& tr1 = d_hoveredSquare.Add<Transform3DComponent>();
    tr1.scale = {0.3f, 0.3f, 0.3f};
    auto& model1 = d_hoveredSquare.Add<ModelComponent>();
    model1.mesh = gridSquare;

    d_selectedSquare = scene.Entities().New();
    auto& n2 = d_selectedSquare.Add<NameComponent>();
    d_selectedSquare.Add<TemporaryComponent>();
    n2.name = "Selected Grid Highlighter";
    auto& tr2 = d_selectedSquare.Add<Transform3DComponent>();
    tr2.scale = {0.5f, 0.5f, 0.5f};
    auto& model2 = d_selectedSquare.Add<ModelComponent>();
    model2.mesh = gridSquare;

    scene.Entities().OnAdd<GridComponent>([&](ecs::Entity entity) {
        auto& transform = entity.Get<Transform3DComponent>();
        const auto& gc = entity.Get<GridComponent>();

        assert(!d_gridEntities.contains({gc.x, gc.z}));
    
        transform.position.x = gc.x + 0.5f;
        transform.position.z = gc.z + 0.5f;
        d_gridEntities[{gc.x, gc.z}] = entity;
    });

    scene.Entities().OnRemove<GridComponent>([&](ecs::Entity entity) {
        auto& gc = entity.Get<GridComponent>();

        auto it = d_gridEntities.find({gc.x, gc.z});
        if (it == d_gridEntities.end()) {
            log::warn("No entity exists at this coord!");
        }
        else {
            d_gridEntities.erase(it);
        }
    });
}

void GameGrid::OnUpdate(Sprocket::Scene&, double dt)
{
    auto& camTr = d_camera.Get<Transform3DComponent>();

    glm::vec3 cameraPos = camTr.position;
    glm::vec3 direction = Maths::GetMouseRay(
        d_window->GetMousePos(),
        d_window->Width(),
        d_window->Height(),
        MakeView(d_camera),
        MakeProj(d_camera)
    );

    float lambda = -cameraPos.y / direction.y;
    glm::vec3 mousePos = cameraPos + lambda * direction;
    d_hovered = {(int)std::floor(mousePos.x), (int)std::floor(mousePos.z)};

    d_hoveredSquare.Get<Transform3DComponent>().position = { d_hovered.x + 0.5f, 0.05f, d_hovered.y + 0.5f };
    if (d_selected.has_value()) {
        d_selectedSquare.Get<Transform3DComponent>().position = { d_selected.value().x + 0.5f, 0.05f, d_selected.value().y + 0.5f };
    } else {
        d_selectedSquare.Get<Transform3DComponent>().position = { 0.5f, -1.0f, 0.5f };
    }
}

void GameGrid::OnEvent(Scene& scene, Event& event)
{
    if (event.IsConsumed()) { return; }

    if (auto e = event.As<MouseButtonPressedEvent>()) {
        if (e->Button() == Mouse::LEFT) {
            d_selected = d_hovered;
        }
        else {
            d_selected = {};
        }
    }
}

ecs::Entity GameGrid::At(const glm::ivec2& pos) const
{
    auto it = d_gridEntities.find(pos);
    if (it != d_gridEntities.end()) {
        return it->second;
    }
    return ecs::Null;
}

ecs::Entity GameGrid::Hovered() const
{
    return At(d_hovered);
}

ecs::Entity GameGrid::Selected() const
{
    if (d_selected.has_value()) {
        return At(d_selected.value());
    }
    return ecs::Null;
}

}