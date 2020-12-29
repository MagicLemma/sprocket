#include "Selector.h"
#include "MouseEvent.h"
#include "Camera.h"
#include "Scene.h"
#include "Components.h"

namespace Sprocket {

Selector::Selector(Window* window, PhysicsEngine* physicsEngine)
    : d_window(window)
    , d_physicsEngine(physicsEngine)
    , d_enabled(false)
    , d_hoveredEntity()
    , d_selectedEntity()
{
}

void Selector::OnStartup(Scene& manager)
{
    manager.Entities().OnRemove<SelectComponent>([&](ECS::Entity entity) {
        if (entity == d_hoveredEntity) { ClearHovered(); }
        if (entity == d_selectedEntity) { ClearSelected(); }
    });
}

void Selector::OnUpdate(Scene& manager, double dt)
{
    if (!d_enabled) {
        ClearHovered();
        ClearSelected();
    }
}

void Selector::OnEvent(Scene& scene, Event& event)
{
    if (!d_enabled) { return; }

    if (auto e = event.As<MouseButtonPressedEvent>()) {
        if (e->IsConsumed()) { return; }
        auto entity = GetMousedOver();
        SetSelected(entity);
        e->Consume();
    }

    else if (auto e = event.As<MouseMovedEvent>()) {  
        ClearHovered(); // Always clear as mouse may now be on GUI
        if (e->IsConsumed()) { return; }
        auto entity = GetMousedOver();
        SetHovered(entity);
        e->Consume();
    }
}

void Selector::Enable(bool newEnabled)
{
    d_enabled = newEnabled;
    ClearHovered();
    ClearSelected();
}


void Selector::ClearHovered()
{
    if (d_hoveredEntity.Valid()) {
        d_hoveredEntity.Get<SelectComponent>().hovered = false;
    }
    d_hoveredEntity = ECS::Null;
}

void Selector::ClearSelected()
{
    if (d_selectedEntity.Valid()) {
        d_selectedEntity.Get<SelectComponent>().selected = false;
    }
    d_selectedEntity = ECS::Null;
}

void Selector::SetHovered(ECS::Entity entity)
{
    ClearHovered();
    if (entity.Valid() && entity.Has<SelectComponent>()) {
        d_hoveredEntity = entity;
        d_hoveredEntity.Get<SelectComponent>().hovered = true;
    }
}

void Selector::SetSelected(ECS::Entity entity)
{
    ClearSelected();
    if (entity.Valid() && entity.Has<SelectComponent>()) {
        d_selectedEntity = entity;
        d_selectedEntity.Get<SelectComponent>().selected = true;
    }
}

ECS::Entity Selector::GetMousedOver()
{
    if (!d_camera.Valid()) { return ECS::Null; }

    auto view = MakeView(d_camera);
    auto proj = MakeProj(d_camera);

    glm::vec3 rayStart = glm::inverse(view) * glm::vec4(0, 0, 0, 1);
    glm::vec3 direction = Maths::GetMouseRay(
        d_window->GetMousePos(),
        d_window->Width(),
        d_window->Height(),
        view,
        proj
    );
    return d_physicsEngine->Raycast(rayStart, direction);
}

}