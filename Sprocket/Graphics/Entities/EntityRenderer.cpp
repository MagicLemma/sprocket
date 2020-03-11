#include "Graphics/Entities/EntityRenderer.h"
#include "Utility/Maths.h"

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
                            const Lights& lights,
                            const RenderOptions& options)
{
    handleRenderOptions(options);
    d_shader.bind();
    unsigned int MAX_NUM_LIGHTS = 5;

    Maths::mat4 projection = camera.projectionMatrix();

    Maths::mat4 view = Maths::createViewMatrix(
        camera.position(),
        camera.pitch(),
        camera.yaw(),
        camera.roll()
    );
    
    d_shader.loadUniform("projectionMatrix", projection);
    d_shader.loadUniform("viewMatrix", view);

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

void EntityRenderer::draw(const Entity& entity)
{
    d_shader.bind();

    // Load up the transform matrix.
	Maths::mat4 transform = entity.transform();
    
    d_shader.loadUniform("transformMatrix", transform);

     // Load remaining entity to shader
	d_shader.loadUniform("shineDamper", entity.texture().shineDamper());
	d_shader.loadUniform("reflectivity", entity.texture().reflectivity());

    entity.bind();
    glDrawElements(GL_TRIANGLES, entity.model().vertexCount(), GL_UNSIGNED_INT, nullptr);
    entity.unbind();

    d_shader.unbind();
}

}