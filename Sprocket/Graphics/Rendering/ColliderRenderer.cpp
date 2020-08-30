#include "ColliderRenderer.h"
#include "Maths.h"
#include "ModelManager.h"
#include "RenderContext.h"
#include "CameraUtils.h"
#include "Components.h"
#include "Scene.h"

#include <glad/glad.h>

namespace Sprocket {

ColliderRenderer::ColliderRenderer(Window* window)
    : d_vao(std::make_unique<VertexArray>())
    , d_window(window)
    , d_shader("Resources/Shaders/Entity.vert", "Resources/Shaders/Entity.frag")
{
}

void ColliderRenderer::Draw(
    const Maths::mat4& proj,
    const Maths::mat4& view,
    const Lights& lights,
    Scene& scene)
{
    RenderContext rc;
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    unsigned int MAX_NUM_LIGHTS = 5;

    d_shader.Bind();
    d_shader.LoadUniform("u_proj_matrix", proj);
    d_shader.LoadUniform("u_view_matrix", view);

    // Load sun to shader
    const auto& sun = scene.GetSun();
    d_shader.LoadUniform("u_sun_direction", sun.direction);
    d_shader.LoadUniform("u_sun_colour", sun.colour);
    d_shader.LoadUniform("u_sun_brightness", sun.brightness);

    // Load ambience to shader
    d_shader.LoadUniform("u_ambience_colour", lights.ambience.colour);
    d_shader.LoadUniform("u_ambience_brightness", lights.ambience.brightness);
    
    // Load point lights to shader
    for (std::size_t i = 0; i != MAX_NUM_LIGHTS; ++i) {
		d_shader.LoadUniform(ArrayName("u_light_pos", i), {0.0f, 0.0f, 0.0f});
        d_shader.LoadUniform(ArrayName("u_light_colour", i), {0.0f, 0.0f, 0.0f});
        d_shader.LoadUniform(ArrayName("u_light_attenuation", i), {1.0f, 0.0f, 0.0f});
        d_shader.LoadUniform(ArrayName("u_light_brightness", i), 0.0f);
	}
    std::size_t i = 0;
    scene.Each<TransformComponent, LightComponent>([&](Entity& entity) {
        if (i < MAX_NUM_LIGHTS) {
            auto position = entity.Get<TransformComponent>().position;
            auto light = entity.Get<LightComponent>();
            d_shader.LoadUniform(ArrayName("u_light_pos", i), position);
			d_shader.LoadUniform(ArrayName("u_light_colour", i), light.colour);
			d_shader.LoadUniform(ArrayName("u_light_attenuation", i), light.attenuation);
            d_shader.LoadUniform(ArrayName("u_light_brightness", i), light.brightness);
        }
        ++i;
    });

    Texture::White().Bind();
    
    static auto s_cube = ModelManager::LoadModel("Resources/Models/Cube.obj");
    d_vao->SetModel(s_cube);
    scene.Each<BoxCollider3DComponent>([&](Entity& entity) {
        const auto& c = entity.Get<BoxCollider3DComponent>();
        auto tr = entity.Get<TransformComponent>();
        Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation);
        transform *= Maths::Transform(c.position, c.orientation);
        transform = Maths::Scale(transform, c.halfExtents);
        if (c.applyScale) {
            transform = Maths::Scale(transform, tr.scale);
        }
        d_shader.LoadUniform("u_model_matrix", transform);
        d_vao->Draw();
    });

    static auto s_sphere = ModelManager::LoadModel("Resources/Models/LowPolySphere.obj");
    d_vao->SetModel(s_sphere);
    scene.Each<SphereCollider3DComponent>([&](Entity& entity) {
        const auto& c = entity.Get<SphereCollider3DComponent>();
        auto tr = entity.Get<TransformComponent>();
        Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation);
        transform *= Maths::Transform(c.position, c.orientation);
        transform = Maths::Scale(transform, c.radius);
        d_shader.LoadUniform("u_model_matrix", transform);
        d_vao->Draw();
    });

    static auto s_hemisphere = ModelManager::LoadModel("Resources/Models/Hemisphere.obj");
    static auto s_cylinder = ModelManager::LoadModel("Resources/Models/Cylinder.obj");
    d_vao->SetModel(s_sphere);
    scene.Each<CapsuleCollider3DComponent>([&](Entity& entity) {
        const auto& c = entity.Get<CapsuleCollider3DComponent>();

        {  // Top Hemisphere
            auto tr = entity.Get<TransformComponent>();
            Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation);
            transform *= Maths::Transform(c.position, c.orientation);
            transform = Maths::Translate(transform, {0.0, c.height/2, 0.0});
            transform = Maths::Scale(transform, c.radius);
            d_shader.LoadUniform("u_model_matrix", transform);
            d_vao->SetModel(s_hemisphere);
            d_vao->Draw();
        }

        {  // Middle Cylinder
            auto tr = entity.Get<TransformComponent>();
            Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation);
            transform *= Maths::Transform(c.position, c.orientation);
            transform = Maths::Scale(transform, {c.radius, c.height, c.radius});
            d_shader.LoadUniform("u_model_matrix", transform);
            d_vao->SetModel(s_cylinder);
            d_vao->Draw();
        }

        {  // Bottom Hemisphere
            auto tr = entity.Get<TransformComponent>();
            Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation);
            transform *= Maths::Transform(c.position, c.orientation);
            transform = Maths::Translate(transform, {0.0, -c.height/2, 0.0});
            transform = Maths::Rotate(transform, {1, 0, 0}, Maths::Radians(180.0f));
            transform = Maths::Scale(transform, c.radius);
            d_shader.LoadUniform("u_model_matrix", transform);
            d_vao->SetModel(s_hemisphere);
            d_vao->Draw();
        }
    });

    d_shader.Unbind();
}

void ColliderRenderer::Draw(const Entity& camera, const Lights& lights, Scene& scene)
{
    Maths::mat4 proj = CameraUtils::MakeProj(camera);
    Maths::mat4 view = CameraUtils::MakeView(camera);
    Draw(proj, view, lights, scene);
}

}