#include "EntityRenderer.h"
#include "Maths.h"
#include "ModelComponent.h"
#include "PositionComponent.h"

#include <glad/glad.h>

namespace Sprocket {

EntityRenderer::EntityRenderer(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/Entity.vert",
               "Resources/Shaders/Entity.frag")
    , d_mirrorShader("Resources/Shaders/EntityMirror.vert",
                     "Resources/Shaders/EntityMirror.frag")
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

void EntityRenderer::draw(std::shared_ptr<Entity> entity, const Texture& tex2)
{
    // Entities without a ModelComponent have nothing to render.
    if (!entity->hasComponent<ModelComponent>()) {
        return;
    }

    d_shader.bind();

    auto modelComp = entity->getComponent<ModelComponent>();

    // If this Entity has a Position component, use the transform on that.
    Maths::mat4 transform = Maths::mat4(1.0f);
    if (entity->hasComponent<PositionComponent>()) {
        transform = entity->getComponent<PositionComponent>().transform();
    }

    d_shader.loadUniform("u_model_matrix", transform);
	d_shader.loadUniform("u_shine_dampner", modelComp.material().shineDamper);
	d_shader.loadUniform("u_reflectivity", modelComp.material().reflectivity);

    modelComp.material().texture.bind(0);
    tex2.bind(1);
    modelComp.model().bind();
    glDrawElements(GL_TRIANGLES, modelComp.model().vertexCount(), GL_UNSIGNED_INT, nullptr);
    modelComp.model().unbind();
    tex2.unbind();
    modelComp.material().texture.unbind();

    d_shader.unbind();
}

}