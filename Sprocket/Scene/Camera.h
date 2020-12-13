#pragma once
#include "ECS.h"
#include "Maths.h"

namespace Sprocket {

glm::mat4 MakeView(const Entity& camera);
    // Returns the view matrix from the entity's position.
    // If the entity has a CameraComponent, the pitch is also
    // taken into account.

glm::mat4 MakeProj(const Entity& camera);
    // Returns the projection matrix from the entity's
    // CameraComponent. If the entity does not have the component,
    // an identity matrix is returned.
    
}