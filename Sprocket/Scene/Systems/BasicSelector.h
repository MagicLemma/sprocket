#pragma once
#include "EntitySystem.h"
#include "ECS.h"

namespace Sprocket {
    
class BasicSelector : public EntitySystem
// A very simple selector system that contains a pointer to a single
// "selected" entity. This can be set at any time and if the selected
// entity is removed from the scene, then this system will automatically
// clear itself.
{
    ECS::Entity d_selectedEntity;

public:
    ~BasicSelector() {}

    void OnStartup(Scene& scene) override;
    
    void SetSelected(ECS::Entity entity);
        // Sets the current selected Entity. If called with a nullptr,
        // it will clear the currently selected Entity.

    ECS::Entity SelectedEntity() const { return d_selectedEntity; }
};

}