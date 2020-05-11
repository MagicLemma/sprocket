#include "EntityRenderer.h"
#include "Maths.h"
#include "Components.h"
#include "ModelLoader.h"

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

}

EntityRenderer::EntityRenderer(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/Entity.vert",
               "Resources/Shaders/Entity.frag")
    , d_outlineShader("Resources/Shaders/EntityOutline.vert",
                      "Resources/Shaders/EntityOutline.frag")
    , d_wireFrame(false)
    , d_depthTest(true)
    , d_renderColliders(false)
{
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void EntityRenderer::wireFrame(bool value)
{
    d_wireFrame = value;
    if (value) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void EntityRenderer::depthTest(bool value)
{
    d_depthTest = value;
    if (value) {
        glEnable(GL_DEPTH_TEST);
    }
    else {
        glDisable(GL_DEPTH_TEST);
    }
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

    d_outlineShader.bind();
    d_outlineShader.loadUniform("u_proj_matrix", lens.projection());
    d_outlineShader.loadUniform("u_view_matrix", camera.view());
    d_outlineShader.unbind();
}

void EntityRenderer::draw(const Entity& entity)
{
    // Entities without a ModelComponent have nothing to render.
    if (!entity.has<ModelComponent>()) {
        return;
    }

    bool outline = false;
    if (entity.has<SelectComponent>()) {
        outline = entity.get<SelectComponent>().hovered ||
                  entity.get<SelectComponent>().selected;
    }

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);

    if (outline) {
        glStencilMask(0xFF);
    }
    else {
        glStencilMask(0x00);
    }

    d_shader.bind();

    auto modelComp = entity.get<ModelComponent>();

    Maths::mat4 transform = entity.transform();
    transform = Maths::scale(transform, modelComp.scale);

    d_shader.loadUniform("u_model_matrix", transform);
	d_shader.loadUniform("u_shine_dampner", modelComp.material.shineDamper);
	d_shader.loadUniform("u_reflectivity", modelComp.material.reflectivity);

    // Bind textures
    bindMaterial(&d_shader, modelComp.material);
    modelComp.model.bind();
    glDrawElements(GL_TRIANGLES, modelComp.model.vertexCount(), GL_UNSIGNED_INT, nullptr);
    modelComp.model.unbind();
    unbindMaterial(&d_shader);

    if (outline) {
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00);
        glDisable(GL_DEPTH_TEST);

        transform = Maths::scale(transform, 1.05f);
        d_outlineShader.bind();

        d_outlineShader.loadUniform("u_model_matrix", transform);

        if (entity.get<SelectComponent>().selected) {
            d_outlineShader.loadUniform("u_colour", Maths::vec4{0, 1, 0, 1});
        }
        else {
            d_outlineShader.loadUniform("u_colour", Maths::vec4{1, 1, 0, 1});
        }

        // Bind textures
        bindMaterial(&d_shader, modelComp.material);
        modelComp.model.bind();
        glDrawElements(GL_TRIANGLES, modelComp.model.vertexCount(), GL_UNSIGNED_INT, nullptr);
        modelComp.model.unbind();
        unbindMaterial(&d_shader);

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_STENCIL_BUFFER_BIT);
    }

    if (d_renderColliders) {
        drawColliders(entity);
    }

    d_shader.unbind();
    d_outlineShader.unbind();
}

void EntityRenderer::drawColliders(const Entity& entity)
{
    if (!entity.has<PhysicsComponent>()) {
        return;
    }

    d_shader.bind();

    ModelLoader loader;
    auto cube = loader.loadModel("Resources/Models/Cube.obj");
    auto sphere = loader.loadModel("Resources/Models/Cube.obj");
    auto hemisphere = loader.loadModel("Resources/Models/Cube.obj");
    auto cylinder = loader.loadModel("Resources/Models/Cube.obj");

    static Model3D s_cube = cube[0].model;
    static Model3D s_sphere = sphere[0].model;
    static Model3D s_hemisphere = hemisphere[0].model;
    static Model3D s_cylinder = cylinder[0].model;

    if (!d_wireFrame) { // Temporarily enable wire framing if off.
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    auto& physicsData = entity.get<PhysicsComponent>();

    if (auto data = std::get_if<BoxCollider>(&physicsData.collider)) {
        Maths::mat4 transform = entity.transform();
        transform = Maths::scale(transform, data->halfExtents);

        s_cube.bind();
        Texture::white().bind();
        d_shader.loadUniform("u_model_matrix", transform);
        glDrawElements(GL_TRIANGLES, s_cube.vertexCount(), GL_UNSIGNED_INT, nullptr);
        Texture::white().unbind();
        s_cube.unbind();
    }
    else if (auto data = std::get_if<SphereCollider>(&physicsData.collider)) {
        Maths::mat4 transform = entity.transform();
        transform = Maths::scale(transform, data->radius);
        
        s_sphere.bind();
        Texture::white().bind();
        d_shader.loadUniform("u_model_matrix", transform);
        glDrawElements(GL_TRIANGLES, s_sphere.vertexCount(), GL_UNSIGNED_INT, nullptr);
        Texture::white().unbind();
        s_sphere.unbind();
    }
    else if (auto data = std::get_if<CapsuleCollider>(&physicsData.collider)) {
        
        Texture::white().bind();

        {  // Top Hemisphere
            s_hemisphere.bind();
            Maths::mat4 transform = entity.transform();
            transform = Maths::translate(transform, {0.0, data->height/2, 0.0});
            transform = Maths::scale(transform, data->radius);
            d_shader.loadUniform("u_model_matrix", transform);
            glDrawElements(GL_TRIANGLES, s_hemisphere.vertexCount(), GL_UNSIGNED_INT, nullptr);
            s_hemisphere.unbind();
        }

        {  // Middle Cylinder
            s_cylinder.bind();
            Maths::mat4 transform = entity.transform();
            transform = Maths::scale(transform, {data->radius, data->height, data->radius});
            d_shader.loadUniform("u_model_matrix", transform);
            glDrawElements(GL_TRIANGLES, s_cylinder.vertexCount(), GL_UNSIGNED_INT, nullptr);
            s_cylinder.unbind();
        }

        {  // Bottom Hemisphere
            s_hemisphere.bind();
            Maths::mat4 transform = entity.transform();
            transform = Maths::translate(transform, {0.0, -data->height/2, 0.0});
            transform = Maths::rotate(transform, {1, 0, 0}, Maths::radians(180.0f));
            transform = Maths::scale(transform, data->radius);
            d_shader.loadUniform("u_model_matrix", transform);
            glDrawElements(GL_TRIANGLES, s_hemisphere.vertexCount(), GL_UNSIGNED_INT, nullptr);
            s_hemisphere.unbind();
        }
        
        Texture::white().unbind();   
    }

    if (!d_wireFrame) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    d_shader.unbind();
}

}