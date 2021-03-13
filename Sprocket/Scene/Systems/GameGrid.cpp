#include "GameGrid.h"
#include "Camera.h"
#include "Components.h"
#include "Scene.h"
#include "MouseCodes.h"
#include "Log.h"

#include <random>
#include <cassert>

namespace Sprocket {

std::string Name(const ecs::Entity& e) {
    if (e.has<NameComponent>()) {
        return e.get<NameComponent>().name;
    }
    return "Entity";
}

GameGrid::GameGrid(Window* window)
    : d_window(window)
    , d_hovered({0.0, 0.0})
    , d_selected({})
{
}

void GameGrid::OnStartup(ecs::Registry& registry)
{
    std::string gridSquare = "Resources/Models/Square.obj";

    d_hoveredSquare = registry.New();
    auto& n1 = d_hoveredSquare.add<NameComponent>();
    d_hoveredSquare.add<TemporaryComponent>();
    n1.name = "Hovered Grid Highlighter";
    auto& tr1 = d_hoveredSquare.add<Transform3DComponent>();
    tr1.scale = {0.3f, 0.3f, 0.3f};
    auto& model1 = d_hoveredSquare.add<ModelComponent>();
    model1.mesh = gridSquare;

    d_selectedSquare = registry.New();
    auto& n2 = d_selectedSquare.add<NameComponent>();
    d_selectedSquare.add<TemporaryComponent>();
    n2.name = "Selected Grid Highlighter";
    auto& tr2 = d_selectedSquare.add<Transform3DComponent>();
    tr2.scale = {0.5f, 0.5f, 0.5f};
    auto& model2 = d_selectedSquare.add<ModelComponent>();
    model2.mesh = gridSquare;
}

void GameGrid::OnUpdate(ecs::Registry&, double dt)
{
    auto& camTr = d_camera.get<Transform3DComponent>();

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

    d_hoveredSquare.get<Transform3DComponent>().position = { d_hovered.x + 0.5f, 0.05f, d_hovered.y + 0.5f };
    if (d_selected.has_value()) {
        d_selectedSquare.get<Transform3DComponent>().position = { d_selected.value().x + 0.5f, 0.05f, d_selected.value().y + 0.5f };
    } else {
        d_selectedSquare.get<Transform3DComponent>().position = { 0.5f, -1.0f, 0.5f };
    }
}

void GameGrid::OnEvent(ecs::Registry&, ev::Event& event)
{
    if (auto data = event.get_if<ecs::ComponentAddedEvent<GridComponent>>()) {
        auto& transform = data->entity.get<Transform3DComponent>();
        const auto& gc = data->entity.get<GridComponent>();

        assert(!d_gridEntities.contains({gc.x, gc.z}));
    
        transform.position.x = gc.x + 0.5f;
        transform.position.z = gc.z + 0.5f;
        d_gridEntities[{gc.x, gc.z}] = data->entity;
    }

    else if (auto data = event.get_if<ecs::ComponentAddedEvent<GridComponent>>()) {
        auto& gc = data->entity.get<GridComponent>();

        auto it = d_gridEntities.find({gc.x, gc.z});
        if (it == d_gridEntities.end()) {
            log::warn("No entity exists at this coord!");
        }
        else {
            d_gridEntities.erase(it);
        }
    }

    if (event.is_consumed()) { return; }

    if (auto e = event.get_if<ev::MouseButtonPressed>()) {
        if (e->button == Mouse::LEFT) {
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