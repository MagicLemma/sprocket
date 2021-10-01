#include "rendering.h"

#include <Sprocket/Graphics/Rendering/geometry_renderer.h>
#include <Sprocket/Graphics/render_context.h>
#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Utility/Maths.h>

#include <glm/glm.hpp>

namespace anvil {
    
void draw_colliders(
    const spkt::geometry_renderer& renderer,
    const spkt::registry& registry,
    const glm::mat4& proj,
    const glm::mat4& view)
{
    spkt::render_context rc;
    rc.wireframe(true);

    renderer.begin_frame(proj, view);

    const auto& make_transform = [](const auto& a, const auto& b) {
        using namespace spkt::Maths;
        return Transform(a.position, a.orientation) * Transform(b.position, b.orientation);
    };

    for (auto [bc, tc] : registry.view_get<spkt::BoxCollider3DComponent, spkt::Transform3DComponent>()) {
        const glm::vec3 scale = bc.applyScale ? bc.halfExtents * tc.scale : bc.halfExtents;      
        renderer.draw_box(make_transform(tc, bc), scale);
    }

    for (auto [sc, tc] : registry.view_get<spkt::SphereCollider3DComponent, spkt::Transform3DComponent>()) {
        renderer.draw_sphere(make_transform(tc, sc), sc.radius);
    }

    for (auto [cc, tc] : registry.view_get<spkt::CapsuleCollider3DComponent, spkt::Transform3DComponent>()) {
        renderer.draw_capsule(make_transform(tc, cc), cc.radius, cc.height);
    }

    renderer.end_frame();
}

}