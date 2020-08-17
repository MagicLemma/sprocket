#include "GameGrid.h"
#include "CameraUtils.h"
#include "Components.h"
#include "EntityManager.h"
#include "MouseEvent.h"
#include "MouseCodes.h"
#include "Log.h"

#include <random>
#include <cassert>

namespace Sprocket {

GameGrid::GameGrid(Window* window)
    : d_window(window)
    , d_hovered({0.0, 0.0})
    , d_selected({})
{
    d_keyboard.ConsumeAll(false);
}

void GameGrid::OnStartup(EntityManager& manager)
{
    std::string gridSquare = "Resources/Models/Square.obj";

    d_hoveredSquare = manager.NewEntity();
    auto& n1 = d_hoveredSquare.Add<NameComponent>();
    d_hoveredSquare.Add<TemporaryComponent>();
    n1.name = "Hovered Grid Highlighter";
    auto& tr1 = d_hoveredSquare.Add<TransformComponent>();
    auto& model1 = d_hoveredSquare.Add<ModelComponent>();
    model1.model = gridSquare;
    model1.reflectivity = 0.0f;
    model1.scale = 0.3f;

    d_selectedSquare = manager.NewEntity();
    auto& n2 = d_selectedSquare.Add<NameComponent>();
    d_selectedSquare.Add<TemporaryComponent>();
    n2.name = "Selected Grid Highlighter";
    auto& tr2 = d_selectedSquare.Add<TransformComponent>();
    auto& model2 = d_selectedSquare.Add<ModelComponent>();
    model2.model = gridSquare;
    model2.reflectivity = 0.0f;
    model2.scale = 0.5f;

    manager.OnAdd<GridComponent>([&](Entity& entity) {
        auto& transform = entity.Get<TransformComponent>();
        const auto& gc = entity.Get<GridComponent>();

        assert(entity.Has<TransformComponent>());
        assert(!d_gridEntities.contains({gc.x, gc.z}));
    
        transform.position.x = gc.x + 0.5f;
        transform.position.z = gc.z + 0.5f;
        d_gridEntities[{gc.x, gc.z}] = entity;
    });
}

void GameGrid::OnUpdate(Sprocket::EntityManager&, double)
{
    auto& camTr = d_camera.Get<TransformComponent>();

    d_mouse.OnUpdate();
    Maths::vec3 cameraPos = camTr.position;
    Maths::vec3 direction = Maths::GetMouseRay(
        d_mouse.GetMousePos(),
        d_window->Width(),
        d_window->Height(),
        CameraUtils::MakeView(d_camera),
        CameraUtils::MakeProj(d_camera)
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
        selected.Kill();
        auto pos = SelectedPosition().value();
        RemoveEntity(pos.x, pos.y);
    }
}

}