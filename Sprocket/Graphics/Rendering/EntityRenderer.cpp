#include "EntityRenderer.h"
#include "Maths.h"
#include "Components.h"

#include <glad/glad.h>

namespace Sprocket {

EntityRenderer::EntityRenderer(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/Entity.vert",
               "Resources/Shaders/Entity.frag")
    , d_mirrorShader("Resources/Shaders/EntityMirror.vert",
                     "Resources/Shaders/EntityMirror.frag")
    , d_cube("Resources/Models/Cube.obj")
    , d_sphere("Resources/Models/LowPolySphere.obj")
    , d_hemisphere("Resources/Models/Hemisphere.obj")
    , d_cylinder("Resources/Models/Cylinder.obj")
{
    d_shader.bind();
    glUniform1i(glGetUniformLocation(d_shader.id(), "texture_sampler[0]"), 0);
    glUniform1i(glGetUniformLocation(d_shader.id(), "texture_sampler[1]"), 1);
    glUniform1i(glGetUniformLocation(d_shader.id(), "texture_sampler[2]"), 2);
    d_shader.unbind();
}

void EntityRenderer::update(const Camera& camera,
                            const Lens& lens,
                            const Lights& lights,
                            const RenderOptions& options)
{
    handleRenderOptions(options);
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

void EntityRenderer::draw(const Entity& entity,
                          bool renderColliders)
{
    // Entities without a ModelComponent have nothing to render.
    if (!entity.has<ModelComponent>()) {
        return;
    }

    d_shader.bind();

    auto modelComp = entity.get<ModelComponent>();

    // If this Entity has a Position component, use the transform on that.
    Maths::mat4 transform = Maths::mat4(1.0f);
    if (entity.has<TransformComponent>()) {
        transform = entity.get<TransformComponent>().transform;
        transform = Maths::scale(transform, modelComp.scale);
    }

    d_shader.loadUniform("u_model_matrix", transform);
	d_shader.loadUniform("u_shine_dampner", modelComp.materials[0].shineDamper);
	d_shader.loadUniform("u_reflectivity", modelComp.materials[0].reflectivity);

    modelComp.materials[0].texture.bind(0);
    modelComp.model.bind();
    glDrawElements(GL_TRIANGLES, modelComp.model.vertexCount(), GL_UNSIGNED_INT, nullptr);
    modelComp.model.unbind();
    modelComp.materials[0].texture.unbind();

    if (renderColliders) {
        drawColliders(entity);
    }

    d_shader.unbind();
}

void EntityRenderer::drawColliders(const Entity& entity)
{
    if (!entity.has<PhysicsComponent>()) {
        return;
    }

    auto& transformData = entity.get<TransformComponent>();
    auto& physicsData = entity.get<PhysicsComponent>();

    if (auto data = std::get_if<BoxCollider>(&physicsData.collider)) {
        Maths::mat4 transform = transformData.transform;
        transform = Maths::scale(transform, data->halfExtents);

        d_cube.bind();
        Texture::white().bind();
        d_shader.loadUniform("u_model_matrix", transform);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
        glDrawElements(GL_TRIANGLES, d_cube.vertexCount(), GL_UNSIGNED_INT, nullptr);
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        Texture::white().unbind();
        d_cube.unbind();
    }
    else if (auto data = std::get_if<SphereCollider>(&physicsData.collider)) {
        Maths::mat4 transform = transformData.transform;
        float radius = data->radius;
        transform = Maths::scale(transform, radius);
        
        d_sphere.bind();
        Texture::white().bind();
        d_shader.loadUniform("u_model_matrix", transform);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
        glDrawElements(GL_TRIANGLES, d_sphere.vertexCount(), GL_UNSIGNED_INT, nullptr);
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        Texture::white().unbind();
        d_sphere.unbind();
    }
    else if (auto data = std::get_if<CapsuleCollider>(&physicsData.collider)) {
        
        Texture::white().bind();
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);

        {  // Top Hemisphere
            d_hemisphere.bind();
            Maths::mat4 transform = transformData.transform;
            transform = Maths::scale(transform, data->radius);
            transform = Maths::translate(transform, {0.0, data->height, 0.0});
            d_shader.loadUniform("u_model_matrix", transform);
            glDrawElements(GL_TRIANGLES, d_hemisphere.vertexCount(), GL_UNSIGNED_INT, nullptr);
            d_hemisphere.unbind();
        }

        {  // Middle Cylinder
            d_cylinder.bind();
            Maths::mat4 transform = transformData.transform;
            float scale = data->radius * data->height;
            transform = Maths::scale(transform, {data->radius, scale, data->radius});
            transform = Maths::translate(transform, {0.0, 0.5, 0.0});
            d_shader.loadUniform("u_model_matrix", transform);
            glDrawElements(GL_TRIANGLES, d_cylinder.vertexCount(), GL_UNSIGNED_INT, nullptr);
            d_cylinder.unbind();
        }

        {  // Bottom Hemisphere
            d_hemisphere.bind();
            Maths::mat4 transform = transformData.transform;
            transform = Maths::scale(transform, data->radius);
            transform = Maths::rotate(transform, {1, 0, 0}, Maths::radians(180.0f));
            d_shader.loadUniform("u_model_matrix", transform);
            glDrawElements(GL_TRIANGLES, d_hemisphere.vertexCount(), GL_UNSIGNED_INT, nullptr);
            d_hemisphere.unbind();
        }
        
        glEnable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        Texture::white().unbind();   
    }
}

}