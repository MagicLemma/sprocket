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

void EntityRenderer::draw(const Entity& entity,
                          const Camera& camera,
                          const Lights& lights,
                          const RenderOptions& options)
{
    handleRenderOptions(options);

    d_shader.bind();
    unsigned int MAX_NUM_LIGHTS = 5;

    // Load up the transform/projection/view matrices.
	Maths::mat4 transform = Maths::createTransformationMatrix(
        entity.position(),               
        entity.rotation(),
        entity.scale());

    Maths::mat4 projection = Sprocket::Maths::createProjectionMatrix(
        d_window->aspectRatio(),
        70.0f,
        0.1f,
        1000.0f);

    Maths::mat4 view = Maths::createViewMatrix(
        camera.position(),
        camera.pitch(),
        camera.yaw(),
        camera.roll()
    );
    
    d_shader.loadUniform("transformMatrix", transform);
    d_shader.loadUniform("projectionMatrix", projection);
    d_shader.loadUniform("viewMatrix", view);

     // Load remaining entity to shader
	d_shader.loadUniform("shineDamper", entity.texture()->shineDamper());
	d_shader.loadUniform("reflectivity", entity.texture()->reflectivity());

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
    

    entity.bind();
    glDrawElements(GL_TRIANGLES, entity.model()->vertexCount(), GL_UNSIGNED_INT, nullptr);
    entity.unbind();

    d_shader.unbind();
}

}