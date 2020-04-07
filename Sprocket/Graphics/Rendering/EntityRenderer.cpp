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
}

void EntityRenderer::update(const Camera& camera,
                            const Lens& lens,
                            const Lights& lights,
                            const RenderOptions& options)
{
    handleRenderOptions(options);
    d_shader.bind();
    unsigned int MAX_NUM_LIGHTS = 5;

    d_shader.loadUniform("projectionMatrix", lens.projection());
    d_shader.loadUniform("viewMatrix", camera.view());

    // Load lights to shader
    for (size_t i = 0; i != MAX_NUM_LIGHTS; ++i) {
		if (i < lights.size()) {
			d_shader.loadUniform(arrayName("lightPositions", i), lights[i].position);
			d_shader.loadUniform(arrayName("lightColours", i), lights[i].colour);
			d_shader.loadUniform(arrayName("lightAttenuations", i), lights[i].attenuation);
		}
		else {  // "Empty" lights to pad the array
			d_shader.loadUniform(arrayName("lightPositions", i), {0.0f, 0.0f, 0.0f});
			d_shader.loadUniform(arrayName("lightColours", i), {0.0f, 0.0f, 0.0f});
			d_shader.loadUniform(arrayName("lightAttenuations", i), {1.0f, 0.0f, 0.0f});
		}
	}
    d_shader.unbind();
}

void EntityRenderer::draw(std::shared_ptr<Entity> entity)
{
    if (!entity->hasComponent<ModelComponent>() || !entity->hasComponent<PositionComponent>()) {
        return;
    }

    d_shader.bind();

    auto modelComp = entity->getComponent<ModelComponent>();
    auto posComp = entity->getComponent<PositionComponent>();

    d_shader.loadUniform("transformMatrix", posComp.transform());
	d_shader.loadUniform("shineDamper", modelComp.material().shineDamper());
	d_shader.loadUniform("reflectivity", modelComp.material().reflectivity());

    modelComp.material().bind();
    modelComp.model().bind();
    glDrawElements(GL_TRIANGLES, modelComp.model().vertexCount(), GL_UNSIGNED_INT, nullptr);
    modelComp.model().unbind();
    modelComp.material().unbind();

    d_shader.unbind();
}

}