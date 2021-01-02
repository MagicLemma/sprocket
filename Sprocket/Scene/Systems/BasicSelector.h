#pragma once
#include "ECS.h"

namespace Sprocket {
    
class BasicSelector
// A very simple selector system that contains a pointer to a single
// "selected" entity. This can be set at any time and if the selected
// entity is removed from the scene, then this system will automatically
// clear itself.
{
    ECS::Entity d_selected;

public:
    ~BasicSelector() {}
    
    void Set(ECS::Entity entity);
    ECS::Entity Get() const;
};

}