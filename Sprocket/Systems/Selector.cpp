#include "Selector.h"
#include "MousePicker.h"
#include "MouseEvent.h"

namespace Sprocket {

Selector::Selector(
    Window* window,
    Camera* camera,
    Lens* lens,
    PhysicsEngine* physicsEngine
)
    : d_window(window)
    , d_camera(camera)
    , d_lens(lens)
    , d_physicsEngine(physicsEngine)
    , d_enabled(false)
    , d_hoveredEntity(nullptr)
    , d_selectedEntity(nullptr)
{
}

void Selector::updateSystem(float dt)
{
    if (!d_enabled) {
        clearHovered();
        clearSelected();
    }
}

void Selector::updateEntity(float dt, Entity& entity)
{
}

bool Selector::handleEvent(Event& event)
{
    if (!d_enabled) { return false; }

    if (auto e = event.as<MouseButtonPressedEvent>()) {
        if (e->isConsumed()) { return false; }
        
        auto entity = getMousedOver();
        setSelected(entity);
        return true;
    }

    else if (auto e = event.as<MouseMovedEvent>()) {  
        clearHovered(); // Always clear as mouse may be on GUI
        
        if (e->isConsumed()) { return false; }
        
        auto entity = getMousedOver();
        setHovered(entity);
        return true;
    }

    return false;
}

void Selector::deregisterEntity(const Entity& entity)
{
    if (&entity == d_hoveredEntity) {
        clearHovered();
    }
    if (&entity == d_selectedEntity) {
        clearSelected();
    }
}

void Selector::enable(bool newEnabled)
{
    d_enabled = newEnabled;
    clearHovered();
    clearSelected();
}


void Selector::clearHovered()
{
    if (d_hoveredEntity != nullptr) {
        d_hoveredEntity->get<SelectComponent>().hovered = false;
        d_hoveredEntity = nullptr;
    }
}

void Selector::clearSelected()
{
    if (d_selectedEntity != nullptr) {
        d_selectedEntity->get<SelectComponent>().selected = false;
        d_selectedEntity = nullptr;
    }
}

void Selector::setHovered(Entity* entity)
{
    clearHovered();
    if (entity != nullptr && entity->has<SelectComponent>()) {
        d_hoveredEntity = entity;
        d_hoveredEntity->get<SelectComponent>().hovered = true;
    }
}

void Selector::setSelected(Entity* entity)
{
    clearSelected();
    if (entity != nullptr && entity->has<SelectComponent>()) {
        d_selectedEntity = entity;
        d_selectedEntity->get<SelectComponent>().selected = true;
    }
}

Entity* Selector::getMousedOver()
{
    Maths::vec3 rayStart = Maths::inverse(d_camera->view()) * Maths::vec4(0, 0, 0, 1);
    Maths::vec3 direction = MousePicker::getRay(d_window, d_camera, d_lens);
    return d_physicsEngine->raycast(rayStart, direction);
}

}