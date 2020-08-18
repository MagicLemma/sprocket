#include "Selector.h"
#include "MouseEvent.h"
#include "CameraUtils.h"
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
    manager.OnRemove<SelectComponent>([&](Entity& entity) {
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

    d_mouse.OnUpdate();
}

void Selector::OnEvent(Event& event)
{
    if (!d_enabled) { return; }

    d_mouse.OnEvent(event);

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
    if (!d_hoveredEntity.Null()) {
        d_hoveredEntity.Get<SelectComponent>().hovered = false;
        d_hoveredEntity = Entity();
    }
}

void Selector::ClearSelected()
{
    if (!d_selectedEntity.Null()) {
        d_selectedEntity.Get<SelectComponent>().selected = false;
        d_selectedEntity = Entity();
    }
}

void Selector::SetHovered(Entity entity)
{
    ClearHovered();
    if (entity.Null()) { return; }

    if (entity.Has<SelectComponent>()) {
        d_hoveredEntity = entity;
        d_hoveredEntity.Get<SelectComponent>().hovered = true;
    }
}

void Selector::SetSelected(Entity entity)
{
    ClearSelected();
    if (!entity.Null() && entity.Has<SelectComponent>()) {
        d_selectedEntity = entity;
        d_selectedEntity.Get<SelectComponent>().selected = true;
    }
}

Entity Selector::GetMousedOver()
{
    if (d_camera.Null()) { return Entity(); }

    auto view = CameraUtils::MakeView(d_camera);
    auto proj = CameraUtils::MakeProj(d_camera);

    Maths::vec3 rayStart = Maths::Inverse(view) * Maths::vec4(0, 0, 0, 1);
    Maths::vec3 direction = Maths::GetMouseRay(
        d_mouse.GetMousePos(),
        d_window->Width(),
        d_window->Height(),
        view,
        proj
    );
    return d_physicsEngine->Raycast(rayStart, direction);
}

}