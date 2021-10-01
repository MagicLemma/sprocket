#include "rendering.h"

#include <Sprocket/Graphics/Rendering/geometry_renderer.h>
#include <Sprocket/Graphics/Rendering/Scene3DRenderer.h>
#include <Sprocket/Graphics/render_context.h>
#include <Sprocket/Scene/ecs.h>
#include <Sprocket/Utility/Maths.h>
#include <Sprocket/Utility/views.h>

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

void draw_scene(
    spkt::Scene3DRenderer& renderer,
    const spkt::registry& registry,
    const glm::mat4& proj,
    const glm::mat4& view)
{
    renderer.begin_frame(proj, view);
    spkt::render_context rc;
    rc.face_culling(true);
    rc.depth_testing(true);

    if (auto a = registry.find<spkt::AmbienceComponent>(); registry.valid(a)) {
        const auto& ambience = registry.get<spkt::AmbienceComponent>(a);
        renderer.set_ambience(ambience.colour, ambience.brightness);
    }

    if (auto s = registry.find<spkt::SunComponent>(); registry.valid(s)) {
        const auto& sun = registry.get<spkt::SunComponent>(s);
        renderer.set_sunlight(sun.colour, sun.direction, sun.brightness);
    }

    for (auto [light, transform] : registry.view_get<spkt::LightComponent, spkt::Transform3DComponent>()
                                 | std::views::take(spkt::MAX_NUM_LIGHTS))
    {
        renderer.add_light(transform.position, light.colour, light.brightness);
    }

    for (auto [mc, tc] : registry.view_get<spkt::StaticModelComponent, spkt::Transform3DComponent>()) {
        renderer.draw_static_mesh(
            tc.position, tc.orientation, tc.scale,
            mc.mesh, mc.material
        );
    }

    for (auto [mc, tc] : registry.view_get<spkt::AnimatedModelComponent, spkt::Transform3DComponent>()) {
        renderer.draw_animated_mesh(
            tc.position, tc.orientation, tc.scale,
            mc.mesh, mc.material,
            mc.animation_name, mc.animation_time
        );
    }

    for (auto [ps] : registry.view_get<spkt::ParticleSingleton>()) {
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