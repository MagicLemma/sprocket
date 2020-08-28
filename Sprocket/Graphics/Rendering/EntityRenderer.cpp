#include "EntityRenderer.h"
#include "Maths.h"
#include "ModelManager.h"
#include "RenderContext.h"
#include "CameraUtils.h"
#include "Components.h"
#include "Scene.h"

#include <glad/glad.h>

namespace Sprocket {
namespace {

bool ShouldOutlineEntity(const Entity& entity)
{
    if (!entity.Has<SelectComponent>()) {
        return false;
    }

    auto& s = entity.Get<SelectComponent>();
    return s.hovered || s.selected;
}

}

EntityRenderer::EntityRenderer(Window* window,
                               ModelManager* modelManager,
                               TextureManager* textureManager)
    : d_window(window)
    , d_modelManager(modelManager)
    , d_textureManager(textureManager)
    , d_shader("Resources/Shaders/Entity.vert", "Resources/Shaders/Entity.frag")
    , d_renderColliders(false)
{
}

void EntityRenderer::RenderColliders(bool value)
{
    d_renderColliders = value;
}

void EntityRenderer::EnableShadows(
    const Texture& shadowMap,
    const Maths::mat4& lightProjView)
{
    glActiveTexture(GL_TEXTURE3);
    shadowMap.Bind();
 
    d_shader.Bind();
    d_shader.LoadUniformInt("shadow_map", 3);
    d_shader.LoadUniform("u_light_proj_view", lightProjView);
    glActiveTexture(GL_TEXTURE0);
}

void EntityRenderer::BeginScene(
    const Maths::mat4& proj,
    const Maths::mat4& view,
    const Lights& lights,
    Scene& scene)
{
    unsigned int MAX_NUM_LIGHTS = 5;

    d_shader.Bind();
    d_shader.LoadUniform("u_proj_matrix", proj);
    d_shader.LoadUniform("u_view_matrix", view);

    // Load sun to shader
    d_shader.LoadUniform("u_sun_direction", lights.sun.direction);
    d_shader.LoadUniform("u_sun_colour", lights.sun.colour);
    d_shader.LoadUniform("u_sun_brightness", lights.sun.brightness);

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
    d_shader.Unbind();
}

void EntityRenderer::BeginScene(const Entity& camera, const Lights& lights, Scene& scene)
{
    Maths::mat4 proj = CameraUtils::MakeProj(camera);
    Maths::mat4 view = CameraUtils::MakeView(camera);
    BeginScene(proj, view, lights, scene);
}

void EntityRenderer::Draw(const Entity& entity)
{    
    RenderContext rc; 

    if (entity.Has<ModelComponent>()) {
        DrawModel(entity);
    }

    if (true || d_renderColliders) {
        DrawCollider(entity);
    }
}

void EntityRenderer::DrawModel(const Entity& entity)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);

    bool outline = ShouldOutlineEntity(entity);
    glStencilMask(outline ? 0xFF : 0x00);

    auto& modelComp = entity.Get<ModelComponent>();

    if (modelComp.model.empty()) {
        return;
    }

    auto tr = entity.Get<TransformComponent>();
    Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation, tr.scale);

    d_shader.Bind();
    d_shader.LoadUniform("u_model_matrix", transform);
	d_shader.LoadUniform("u_shine_dampner", modelComp.shineDamper);
	d_shader.LoadUniform("u_reflectivity", modelComp.reflectivity);

    float brightness = 1.0f;
    if (entity.Has<SelectComponent>()) {
        auto data = entity.Get<SelectComponent>();
        if (data.selected) {
            brightness = 1.5f;
        } else if (data.hovered) {
            brightness = 1.25f;
        }
    }
    d_shader.LoadUniform("u_brightness", brightness);

    auto model = d_modelManager->GetModel(modelComp.model);
    auto texture = d_textureManager->GetTexture(modelComp.texture);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();
    model.Bind();
    glDrawElements(GL_TRIANGLES, (int)model.VertexCount(), GL_UNSIGNED_INT, nullptr);
    model.Unbind();
    texture.Unbind();
}

void EntityRenderer::DrawCollider(const Entity& entity)
{
    RenderContext rc;  // New render context for colliders.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    d_shader.Bind();
    if (entity.Has<BoxCollider3DComponent>()) {
        DrawBox(entity);
    }
    if (entity.Has<SphereCollider3DComponent>()) {
        DrawSphere(entity);
    }
    if (entity.Has<CapsuleCollider3DComponent>()) {
        DrawCapsule(entity);
    }
    d_shader.Unbind();
}

void EntityRenderer::DrawBox(const Entity& entity)
{
    const auto& physics = entity.Get<BoxCollider3DComponent>();
    static auto s_cube = ModelManager::LoadModel("Resources/Models/Cube.obj");

    auto tr = entity.Get<TransformComponent>();
    Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation);
    transform *= Maths::Transform(physics.position, physics.orientation);
    transform = Maths::Scale(transform, physics.halfExtents);

    s_cube.Bind();
    Texture::White().Bind();
    d_shader.LoadUniform("u_model_matrix", transform);
    glDrawElements(GL_TRIANGLES, (int)s_cube.VertexCount(), GL_UNSIGNED_INT, nullptr);
    Texture::White().Unbind();
    s_cube.Unbind();
}

void EntityRenderer::DrawSphere(const Entity& entity)
{
    const auto& physics = entity.Get<SphereCollider3DComponent>();
    static auto s_sphere = ModelManager::LoadModel("Resources/Models/LowPolySphere.obj");

    auto tr = entity.Get<TransformComponent>();
    Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation);
    transform *= Maths::Transform(physics.position, physics.orientation);
    transform = Maths::Scale(transform, physics.radius);
    
    s_sphere.Bind();
    Texture::White().Bind();
    d_shader.LoadUniform("u_model_matrix", transform);
    glDrawElements(GL_TRIANGLES, (int)s_sphere.VertexCount(), GL_UNSIGNED_INT, nullptr);
    Texture::White().Unbind();
    s_sphere.Unbind();
}

void EntityRenderer::DrawCapsule(const Entity& entity)
{
    const auto& physics = entity.Get<CapsuleCollider3DComponent>();
    static auto s_hemisphere = ModelManager::LoadModel("Resources/Models/Hemisphere.obj");
    static auto s_cylinder = ModelManager::LoadModel("Resources/Models/Cylinder.obj");

    Texture::White().Bind();
    {  // Top Hemisphere
        s_hemisphere.Bind();
        auto tr = entity.Get<TransformComponent>();
        Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation);
        transform *= Maths::Transform(physics.position, physics.orientation);
        transform = Maths::Translate(transform, {0.0, physics.height/2, 0.0});
        transform = Maths::Scale(transform, physics.radius);
        d_shader.LoadUniform("u_model_matrix", transform);
        glDrawElements(GL_TRIANGLES, (int)s_hemisphere.VertexCount(), GL_UNSIGNED_INT, nullptr);
        s_hemisphere.Unbind();
    }

    {  // Middle Cylinder
        s_cylinder.Bind();
        auto tr = entity.Get<TransformComponent>();
        Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation);
        transform *= Maths::Transform(physics.position, physics.orientation);
        transform = Maths::Scale(transform, {physics.radius, physics.height, physics.radius});
        d_shader.LoadUniform("u_model_matrix", transform);
        glDrawElements(GL_TRIANGLES, (int)s_cylinder.VertexCount(), GL_UNSIGNED_INT, nullptr);
        s_cylinder.Unbind();
    }

    {  // Bottom Hemisphere
        s_hemisphere.Bind();
        auto tr = entity.Get<TransformComponent>();
        Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation);
        transform *= Maths::Transform(physics.position, physics.orientation);
        transform = Maths::Translate(transform, {0.0, -physics.height/2, 0.0});
        transform = Maths::Rotate(transform, {1, 0, 0}, Maths::Radians(180.0f));
        transform = Maths::Scale(transform, physics.radius);
        d_shader.LoadUniform("u_model_matrix", transform);
        glDrawElements(GL_TRIANGLES, (int)s_hemisphere.VertexCount(), GL_UNSIGNED_INT, nullptr);
        s_hemisphere.Unbind();
    }
    Texture::White().Unbind();   
}

}