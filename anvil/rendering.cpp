#include "rendering.h"
#include <anvil/ecs/ecs.h>

#include <sprocket/graphics/Rendering/geometry_renderer.h>
#include <sprocket/graphics/Rendering/pbr_renderer.h>
#include <sprocket/graphics/render_context.h>
#include <sprocket/Utility/maths.h>
#include <sprocket/Utility/views.h>

#include <glm/glm.hpp>

namespace anvil {
    
void draw_colliders(
    const spkt::geometry_renderer& renderer,
    const anvil::registry& registry,
    const glm::mat4& proj,
    const glm::mat4& view)
{
    spkt::render_context rc;
    rc.wireframe(true);

    renderer.begin_frame(proj, view);

    const auto& transform = [](const auto& a, const auto& b) {
        return spkt::make_transform(a.position, a.orientation)
             * spkt::make_transform(b.position, b.orientation);
    };

    for (auto [bc, tc] : registry.view_get<anvil::BoxCollider3DComponent, anvil::Transform3DComponent>()) {
        const glm::vec3 scale = bc.applyScale ? bc.halfExtents * tc.scale : bc.halfExtents;      
        renderer.draw_box(transform(tc, bc), scale);
    }

    for (auto [sc, tc] : registry.view_get<anvil::SphereCollider3DComponent, anvil::Transform3DComponent>()) {
        renderer.draw_sphere(transform(tc, sc), sc.radius);
    }

    for (auto [cc, tc] : registry.view_get<anvil::CapsuleCollider3DComponent, anvil::Transform3DComponent>()) {
        renderer.draw_capsule(transform(tc, cc), cc.radius, cc.height);
    }

    renderer.end_frame();
}

void draw_scene(
    spkt::pbr_renderer& renderer,
    const anvil::registry& registry,
    const glm::mat4& proj,
    const glm::mat4& view)
{
    renderer.begin_frame(proj, view);
    spkt::render_context rc;
    rc.face_culling(true);
    rc.depth_testing(true);

    for (auto [ac] : registry.view_get<anvil::AmbienceComponent>()) {
        renderer.set_ambience(ac.colour, ac.brightness);
    }

    for (auto [sc] : registry.view_get<anvil::SunComponent>()) {
        renderer.set_sunlight(sc.colour, sc.direction, sc.brightness);
    }

    for (auto [lc, tc] : registry.view_get<anvil::LightComponent, anvil::Transform3DComponent>()
                                 | std::views::take(spkt::MAX_NUM_LIGHTS))
    {
        renderer.add_light(tc.position, lc.colour, lc.brightness);
    }

    for (auto [mc, tc] : registry.view_get<anvil::StaticModelComponent, anvil::Transform3DComponent>()) {
        renderer.draw_static_mesh(
            tc.position, tc.orientation, tc.scale,
            mc.mesh, mc.material
        );
    }

    for (auto [mc, tc] : registry.view_get<anvil::AnimatedModelComponent, anvil::Transform3DComponent>()) {
        renderer.draw_animated_mesh(
            tc.position, tc.orientation, tc.scale,
            mc.mesh, mc.material, mc.animation_name, mc.animation_time
        );
    }

    for (auto [ps] : registry.view_get<anvil::ParticleSingleton>()) {
        std::vector<spkt::model_instance> instance_data(spkt::NUM_PARTICLES);
        for (const auto& particle : *ps.particles) {
            if (particle.life > 0.0) {
                instance_data.push_back({particle.position, {0.0, 0.0, 0.0, 1.0}, particle.scale});
            }
        }
        renderer.draw_particles(instance_data);
    };

    renderer.end_frame();
}

}