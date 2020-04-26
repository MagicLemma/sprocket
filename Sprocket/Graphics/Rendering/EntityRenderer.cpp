#include "EntityRenderer.h"
#include "Maths.h"
#include "Components.h"

#include <glad/glad.h>

namespace Sprocket {

EntityRenderer::EntityRenderer(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/Entity.vert",
               "Resources/Shaders/Entity.frag")
    , d_wireFrame(false)
    , d_depthTest(true)
    , d_renderColliders(false)
{
    d_shader.bind();
    glUniform1i(glGetUniformLocation(d_shader.id(), "texture_sampler[0]"), 0);
    glUniform1i(glGetUniformLocation(d_shader.id(), "texture_sampler[1]"), 1);
    glUniform1i(glGetUniformLocation(d_shader.id(), "texture_sampler[2]"), 2);
    d_shader.unbind();
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
}

void EntityRenderer::draw(const Entity& entity)
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

    if (d_renderColliders) {
        drawColliders(entity);
    }

    d_shader.unbind();
}

void EntityRenderer::drawColliders(const Entity& entity)
{
    if (!entity.has<PhysicsComponent>()) {
        return;
    }

    static Model3D s_cube("Resources/Models/Cube.obj");
    static Model3D s_sphere("Resources/Models/LowPolySphere.obj");
    static Model3D s_hemisphere("Resources/Models/Hemisphere.obj");
    static Model3D s_cylinder("Resources/Models/Cylinder.obj");

    if (!d_wireFrame) { // Temporarily enable wire framing if off.
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    if (d_depthTest) { // Temporarily disable depth testing if on.
        glDisable(GL_CULL_FACE);
    }

    auto& transformData = entity.get<TransformComponent>();
    auto& physicsData = entity.get<PhysicsComponent>();

    if (auto data = std::get_if<BoxCollider>(&physicsData.collider)) {
        Maths::mat4 transform = transformData.transform;
        transform = Maths::scale(transform, data->halfExtents);

        s_cube.bind();
        Texture::white().bind();
        d_shader.loadUniform("u_model_matrix", transform);
        glDisable(GL_CULL_FACE);
        glDrawElements(GL_TRIANGLES, s_cube.vertexCount(), GL_UNSIGNED_INT, nullptr);
        glEnable(GL_CULL_FACE);
        Texture::white().unbind();
        s_cube.unbind();
    }
    else if (auto data = std::get_if<SphereCollider>(&physicsData.collider)) {
        Maths::mat4 transform = transformData.transform;
        float radius = data->radius;
        transform = Maths::scale(transform, radius);
        
        s_sphere.bind();
        Texture::white().bind();
        d_shader.loadUniform("u_model_matrix", transform);
        glDisable(GL_CULL_FACE);
        glDrawElements(GL_TRIANGLES, s_sphere.vertexCount(), GL_UNSIGNED_INT, nullptr);
        glEnable(GL_CULL_FACE);
        Texture::white().unbind();
        s_sphere.unbind();
    }
    else if (auto data = std::get_if<CapsuleCollider>(&physicsData.collider)) {
        
        Texture::white().bind();
        glDisable(GL_CULL_FACE);

        {  // Top Hemisphere
            s_hemisphere.bind();
            Maths::mat4 transform = transformData.transform;
            transform = Maths::translate(transform, {0.0, data->height/2, 0.0});
            transform = Maths::scale(transform, data->radius);
            d_shader.loadUniform("u_model_matrix", transform);
            glDrawElements(GL_TRIANGLES, s_hemisphere.vertexCount(), GL_UNSIGNED_INT, nullptr);
            s_hemisphere.unbind();
        }

        {  // Middle Cylinder
            s_cylinder.bind();
            Maths::mat4 transform = transformData.transform;
            transform = Maths::scale(transform, {data->radius, data->height, data->radius});
            d_shader.loadUniform("u_model_matrix", transform);
            glDrawElements(GL_TRIANGLES, s_cylinder.vertexCount(), GL_UNSIGNED_INT, nullptr);
            s_cylinder.unbind();
        }

        {  // Bottom Hemisphere
            s_hemisphere.bind();
            Maths::mat4 transform = transformData.transform;
            transform = Maths::translate(transform, {0.0, -data->height/2, 0.0});
            transform = Maths::rotate(transform, {1, 0, 0}, Maths::radians(180.0f));
            transform = Maths::scale(transform, data->radius);
            d_shader.loadUniform("u_model_matrix", transform);
            glDrawElements(GL_TRIANGLES, s_hemisphere.vertexCount(), GL_UNSIGNED_INT, nullptr);
            s_hemisphere.unbind();
        }
        
        glEnable(GL_CULL_FACE);
        Texture::white().unbind();   
    }

    if (!d_wireFrame) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (d_depthTest) {
        glEnable(GL_CULL_FACE);
    }
}

}