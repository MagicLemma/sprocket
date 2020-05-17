#include "TerrainRenderer.h"
#include "Maths.h"
#include "RenderContext.h"

#include <glad/glad.h>

namespace Sprocket {

TerrainRenderer::TerrainRenderer(Window* window)
    : d_window(window)
    , d_shader("Resources/Shaders/Terrain.vert",
               "Resources/Shaders/Terrain.frag")
{

}

void TerrainRenderer::update(const Camera& camera,
                             const Lens& lens,
                             const Lights& lights)
{
    d_shader.bind();
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

void TerrainRenderer::draw(const Terrain& terrain)
{
    RenderContext rc;
    d_shader.bind();

    // Load up the transform matrix.
	Maths::mat4 transform = Maths::transform(terrain.position(), Maths::identity);
    
    d_shader.loadUniform("transformMatrix", transform);
	d_shader.loadUniform("shineDamper", terrain.material().shineDamper);
	d_shader.loadUniform("reflectivity", terrain.material().reflectivity);
    
    terrain.bind();
    glDrawElements(GL_TRIANGLES, terrain.model().vertexCount(), GL_UNSIGNED_INT, nullptr);
    terrain.unbind();

    d_shader.unbind();
}

}