#include "EntityRenderer.h"
#include "Maths.h"
#include "Components.h"
#include "ModelManager.h"
#include "RenderContext.h"

#include <glad/glad.h>

namespace Sprocket {
namespace {

void bindMaterial(Shader* shader, const Material& material)
{
    glActiveTexture(GL_TEXTURE0);
    material.texture.bind();
    shader->loadUniformInt("texture_sampler", 0);

    glActiveTexture(GL_TEXTURE1);
    material.specularMap.bind();
    shader->loadUniformInt("specular_sampler", 1);

    glActiveTexture(GL_TEXTURE2);
    material.normalMap.bind();
    shader->loadUniformInt("normal_sampler", 2);

    glActiveTexture(GL_TEXTURE0);
}

void unbindMaterial(Shader* shader)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);

    glActiveTexture(GL_TEXTURE0);
}

bool shouldOutlineEntity(const Entity& entity)
{
    if (!entity.has<SelectComponent>()) {
        return false;
    }

    auto& s = entity.get<SelectComponent>();
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

void EntityRenderer::renderColliders(bool value)
{
    d_renderColliders = value;
}

void EntityRenderer::update(const Camera& camera,
                            const Lens& lens,
                            const Lights& lights)
{
    d_shader.bind();
    unsigned int MAX_NUM_LIGHTS = 5;

    d_shader.loadUniform("u_proj_matrix", lens.projection());
    d_shader.loadUniform("u_view_matrix", camera.view());

    // Load lights to shader
    for (size_t i = 0; i != MAX_NUM_LIGHTS; ++i) {
		if (i < lights.size()) {
			d_shader.loadUniform(arrayName("u_light_pos", i), lights[i].position);
			d_shader.loadUniform(arrayName("u_light_colour", i), lights[i].colour);
			d_shader.loadUniform(arrayName("u_light_attenuation", i), lights[i].attenuation);
		}
		else {  // "Empty" lights to pad the array
			d_shader.loadUniform(arrayName("u_light_pos", i), {0.0f, 0.0f, 0.0f});
			d_shader.loadUniform(arrayName("u_light_colour", i), {0.0f, 0.0f, 0.0f});
			d_shader.loadUniform(arrayName("u_light_attenuation", i), {1.0f, 0.0f, 0.0f});
		}
	}
    d_shader.unbind();
}

void EntityRenderer::draw(const Entity& entity)
{    
    RenderContext rc; 

    if (entity.has<ModelComponent>()) {
        drawModel(entity);
    }

    if (d_renderColliders && entity.has<ColliderComponent>()) {
        drawCollider(entity);
    }
}

void EntityRenderer::drawModel(const Entity& entity)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);

    bool outline = shouldOutlineEntity(entity);
    glStencilMask(outline ? 0xFF : 0x00);

    auto modelComp = entity.get<ModelComponent>();
    Maths::mat4 transform = entity.transform();
    transform = Maths::scale(transform, modelComp.scale);

    d_shader.bind();
    d_shader.loadUniform("u_model_matrix", transform);
	d_shader.loadUniform("u_shine_dampner", modelComp.material.shineDamper);
	d_shader.loadUniform("u_reflectivity", modelComp.material.reflectivity);

    if (entity.has<SelectComponent>()) {
        auto data = entity.get<SelectComponent>();
        if (data.selected) {
            d_shader.loadUniform("u_brightness", 1.5f);
        }   else if (data.hovered) {
            d_shader.loadUniform("u_brightness", 1.25f);
        }
        else {
            d_shader.loadUniform("u_brightness", 1.0f);
        }
    } else {
        d_shader.loadUniform("u_brightness", 1.0f);
    }

    // Bind textures
    bindMaterial(&d_shader, modelComp.material);
    modelComp.model.bind();
    glDrawElements(GL_TRIANGLES, (int)modelComp.model.vertexCount(), GL_UNSIGNED_INT, nullptr);
    modelComp.model.unbind();
    unbindMaterial(&d_shader);
}

void EntityRenderer::drawCollider(const Entity& entity)
{
    RenderContext rc;  // New render context for colliders.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    d_shader.bind();
    auto& colliderData = entity.get<ColliderComponent>();
    if (auto data = std::get_if<BoxCollider>(&colliderData.collider)) {
        drawBox(entity, data);
    }
    else if (auto data = std::get_if<SphereCollider>(&colliderData.collider)) {
        drawSphere(entity, data);
    }
    else if (auto data = std::get_if<CapsuleCollider>(&colliderData.collider)) {
        drawCapsule(entity, data);
    }
    d_shader.unbind();
}

void EntityRenderer::drawBox(const Entity& entity, const BoxCollider* collider)
{
    static auto s_cube = ModelManager::loadModel("Resources/Models/Cube.obj");

    Maths::mat4 transform = entity.transform();
    transform = Maths::scale(transform, collider->halfExtents);

    s_cube.bind();
    Texture::white().bind();
    d_shader.loadUniform("u_model_matrix", transform);
    glDrawElements(GL_TRIANGLES, (int)s_cube.vertexCount(), GL_UNSIGNED_INT, nullptr);
    Texture::white().unbind();
    s_cube.unbind();
}

void EntityRenderer::drawSphere(const Entity& entity, const SphereCollider* collider)
{
    static auto s_sphere = ModelManager::loadModel("Resources/Models/LowPolySphere.obj");

    Maths::mat4 transform = entity.transform();
    transform = Maths::scale(transform, collider->radius);
    
    s_sphere.bind();
    Texture::white().bind();
    d_shader.loadUniform("u_model_matrix", transform);
    glDrawElements(GL_TRIANGLES, (int)s_sphere.vertexCount(), GL_UNSIGNED_INT, nullptr);
    Texture::white().unbind();
    s_sphere.unbind();
}

void EntityRenderer::drawCapsule(const Entity& entity, const CapsuleCollider* collider)
{
    static auto s_hemisphere = ModelManager::loadModel("Resources/Models/Hemisphere.obj");
    static auto s_cylinder = ModelManager::loadModel("Resources/Models/Cylinder.obj");

    Texture::white().bind();
    {  // Top Hemisphere
        s_hemisphere.bind();
        Maths::mat4 transform = entity.transform();
        transform = Maths::translate(transform, {0.0, collider->height/2, 0.0});
        transform = Maths::scale(transform, collider->radius);
        d_shader.loadUniform("u_model_matrix", transform);
        glDrawElements(GL_TRIANGLES, (int)s_hemisphere.vertexCount(), GL_UNSIGNED_INT, nullptr);
        s_hemisphere.unbind();
    }

    {  // Middle Cylinder
        s_cylinder.bind();
        Maths::mat4 transform = entity.transform();
        transform = Maths::scale(transform, {collider->radius, collider->height, collider->radius});
        d_shader.loadUniform("u_model_matrix", transform);
        glDrawElements(GL_TRIANGLES, (int)s_cylinder.vertexCount(), GL_UNSIGNED_INT, nullptr);
        s_cylinder.unbind();
    }

    {  // Bottom Hemisphere
        s_hemisphere.bind();
        Maths::mat4 transform = entity.transform();
        transform = Maths::translate(transform, {0.0, -collider->height/2, 0.0});
        transform = Maths::rotate(transform, {1, 0, 0}, Maths::radians(180.0f));
        transform = Maths::scale(transform, collider->radius);
        d_shader.loadUniform("u_model_matrix", transform);
        glDrawElements(GL_TRIANGLES, (int)s_hemisphere.vertexCount(), GL_UNSIGNED_INT, nullptr);
        s_hemisphere.unbind();
    }
    Texture::white().unbind();   
}

}