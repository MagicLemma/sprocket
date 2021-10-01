#pragma once
#include <Sprocket/Graphics/Rendering/geometry_renderer.h>
#include <Sprocket/Graphics/Rendering/Scene3DRenderer.h>
#include <Sprocket/Scene/ecs.h>

#include <glm/glm.hpp>

namespace anvil {

void draw_colliders(
    const spkt::geometry_renderer& renderer,
    const spkt::registry& registry,
    const glm::mat4& proj,
    const glm::mat4& view
);

void draw_scene(
    spkt::Scene3DRenderer& renderer,
    const spkt::registry& registry,
    const glm::mat4& proj,
    const glm::mat4& view
);

}