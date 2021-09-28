#pragma once
#include <Sprocket/Scene/ecs.h>

namespace spkt {

// Returns the view matrix from the entity's position.
// If the entity has a CameraComponent, the pitch is also
// taken into account.
glm::mat4 make_view(const spkt::registry& registry, spkt::entity camera);
glm::mat4 make_view(const glm::vec3& pos, const glm::quat& ori, float pitch);

// Returns the projection matrix from the entity's
// CameraComponent. If the entity does not have the component,
// an identity matrix is returned.
glm::mat4 make_proj(const spkt::registry& registry, spkt::entity camera);
glm::mat4 make_proj(float fov);
    
}