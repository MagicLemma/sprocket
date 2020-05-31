#include "EntityRenderer.h"
#include "Maths.h"
#include "Components.h"
#include "ModelManager.h"
#include "RenderContext.h"

#include <glad/glad.h>

namespace Sprocket {
namespace {

void BindMaterial(Shader* shader, const Material& material)
{
    glActiveTexture(GL_TEXTURE0);
    material.texture.Bind();
    shader->LoadUniformInt("texture_sampler", 0);

    glActiveTexture(GL_TEXTURE1);
    material.specularMap.Bind();
    shader->LoadUniformInt("specular_sampler", 1);

    glActiveTexture(GL_TEXTURE2);
    material.normalMap.Bind();
    shader->LoadUniformInt("normal_sampler", 2);

    glActiveTexture(GL_TEXTURE0);
}

void UnbindMaterial(Shader* shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE0);
}

bool ShouldOutlineEntity(const Entity& entity)
{
    if (!entity.Has<SelectComponent>()) {
        return false;
    }

    auto& s = entity.Get<SelectComponent>();
    return s.hovered || s.selected;
}

}

EntityRenderer::EntityRenderer(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/Entity.vert",
               "Resources/Shaders/Entity.frag")
    , d_renderColliders(false)
{
}

void EntityRenderer::RenderColliders(bool value)
{
    d_renderColliders = value;
}

void EntityRenderer::OnUpdate(const Camera& camera,
                            const Lens& lens,
                            const Lights& lights)
{
    d_shader.Bind();
    unsigned int MAX_NUM_LIGHTS = 5;

    d_shader.LoadUniform("u_proj_matrix", lens.Projection());
    d_shader.LoadUniform("u_view_matrix", camera.View());

    // Load lights to shader
    for (size_t i = 0; i != MAX_NUM_LIGHTS; ++i) {
		if (i < lights.size()) {
			d_shader.LoadUniform(ArrayName("u_light_pos", i), lights[i].position);
			d_shader.LoadUniform(ArrayName("u_light_colour", i), lights[i].colour);
			d_shader.LoadUniform(ArrayName("u_light_attenuation", i), lights[i].attenuation);
		}
		else {  // "Empty" lights to pad the array
			d_shader.LoadUniform(ArrayName("u_light_pos", i), {0.0f, 0.0f, 0.0f});
			d_shader.LoadUniform(ArrayName("u_light_colour", i), {0.0f, 0.0f, 0.0f});
			d_shader.LoadUniform(ArrayName("u_light_attenuation", i), {1.0f, 0.0f, 0.0f});
		}
	}
    d_shader.Unbind();
}

void EntityRenderer::Draw(const Entity& entity)
{    
    RenderContext rc; 

    if (entity.Has<ModelComponent>()) {
        DrawModel(entity);
    }

    if (d_renderColliders && entity.Has<ColliderComponent>()) {
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

    auto modelComp = entity.Get<ModelComponent>();
    Maths::mat4 transform = entity.Transform();
    transform = Maths::Scale(transform, modelComp.scale);

    d_shader.Bind();
    d_shader.LoadUniform("u_model_matrix", transform);
	d_shader.LoadUniform("u_shine_dampner", modelComp.material.shineDamper);
	d_shader.LoadUniform("u_reflectivity", modelComp.material.reflectivity);

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

    // Bind textures
    BindMaterial(&d_shader, modelComp.material);
    modelComp.model.Bind();
    glDrawElements(GL_TRIANGLES, (int)modelComp.model.VertexCount(), GL_UNSIGNED_INT, nullptr);
    modelComp.model.Unbind();
    UnbindMaterial(&d_shader);
}

void EntityRenderer::DrawCollider(const Entity& entity)
{
    RenderContext rc;  // New render context for colliders.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    d_shader.Bind();
    auto& colliderData = entity.Get<ColliderComponent>();
    if (auto data = std::get_if<BoxCollider>(&colliderData.collider)) {
        DrawBox(entity, data);
    }
    else if (auto data = std::get_if<SphereCollider>(&colliderData.collider)) {
        DrawSphere(entity, data);
    }
    else if (auto data = std::get_if<CapsuleCollider>(&colliderData.collider)) {
        DrawCapsule(entity, data);
    }
    d_shader.Unbind();
}

void EntityRenderer::DrawBox(const Entity& entity, const BoxCollider* collider)
{
    static auto s_cube = ModelManager::LoadModel("Resources/Models/Cube.obj");

    Maths::mat4 transform = entity.Transform();
    transform = Maths::Scale(transform, collider->halfExtents);

    s_cube.Bind();
    Texture::White().Bind();
    d_shader.LoadUniform("u_model_matrix", transform);
    glDrawElements(GL_TRIANGLES, (int)s_cube.VertexCount(), GL_UNSIGNED_INT, nullptr);
    Texture::White().Unbind();
    s_cube.Unbind();
}

void EntityRenderer::DrawSphere(const Entity& entity, const SphereCollider* collider)
{
    static auto s_sphere = ModelManager::LoadModel("Resources/Models/LowPolySphere.obj");

    Maths::mat4 transform = entity.Transform();
    transform = Maths::Scale(transform, collider->radius);
    
    s_sphere.Bind();
    Texture::White().Bind();
    d_shader.LoadUniform("u_model_matrix", transform);
    glDrawElements(GL_TRIANGLES, (int)s_sphere.VertexCount(), GL_UNSIGNED_INT, nullptr);
    Texture::White().Unbind();
    s_sphere.Unbind();
}

void EntityRenderer::DrawCapsule(const Entity& entity, const CapsuleCollider* collider)
{
    static auto s_hemisphere = ModelManager::LoadModel("Resources/Models/Hemisphere.obj");
    static auto s_cylinder = ModelManager::LoadModel("Resources/Models/Cylinder.obj");

    Texture::White().Bind();
    {  // Top Hemisphere
        s_hemisphere.Bind();
        Maths::mat4 transform = entity.Transform();
        transform = Maths::Translate(transform, {0.0, collider->height/2, 0.0});
        transform = Maths::Scale(transform, collider->radius);
        d_shader.LoadUniform("u_model_matrix", transform);
        glDrawElements(GL_TRIANGLES, (int)s_hemisphere.VertexCount(), GL_UNSIGNED_INT, nullptr);
        s_hemisphere.Unbind();
    }

    {  // Middle Cylinder
        s_cylinder.Bind();
        Maths::mat4 transform = entity.Transform();
        transform = Maths::Scale(transform, {collider->radius, collider->height, collider->radius});
        d_shader.LoadUniform("u_model_matrix", transform);
        glDrawElements(GL_TRIANGLES, (int)s_cylinder.VertexCount(), GL_UNSIGNED_INT, nullptr);
        s_cylinder.Unbind();
    }

    {  // Bottom Hemisphere
        s_hemisphere.Bind();
        Maths::mat4 transform = entity.Transform();
        transform = Maths::Translate(transform, {0.0, -collider->height/2, 0.0});
        transform = Maths::Rotate(transform, {1, 0, 0}, Maths::Radians(180.0f));
        transform = Maths::Scale(transform, collider->radius);
        d_shader.LoadUniform("u_model_matrix", transform);
        glDrawElements(GL_TRIANGLES, (int)s_hemisphere.VertexCount(), GL_UNSIGNED_INT, nullptr);
        s_hemisphere.Unbind();
    }
    Texture::White().Unbind();   
}

}