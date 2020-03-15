#include "Graphics/Terrains/TerrainRenderer.h"
#include "Utility/Maths.h"

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

void TerrainRenderer::draw(const Terrain& terrain)
{
    d_shader.bind();

    // Load up the transform matrix.
	Maths::mat4 transform = Maths::transform(
        terrain.position(),               
        Maths::vec3(0.0f),
        1.0f);
    
    d_shader.loadUniform("transformMatrix", transform);
	d_shader.loadUniform("shineDamper", terrain.texture().shineDamper());
	d_shader.loadUniform("reflectivity", terrain.texture().reflectivity());
    
    terrain.bind();
    glDrawElements(GL_TRIANGLES, terrain.model().vertexCount(), GL_UNSIGNED_INT, nullptr);
    terrain.unbind();

    d_shader.unbind();
}

}