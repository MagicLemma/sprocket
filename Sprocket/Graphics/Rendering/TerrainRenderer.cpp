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

void TerrainRenderer::OnUpdate(const Camera& camera,
                               const Lens& lens,
                               const PointLights& lights)
{
    d_shader.Bind();
    d_shader.LoadUniform("projectionMatrix", lens.Projection());
    d_shader.LoadUniform("viewMatrix", camera.View());

    // Load lights to shader
    for (size_t i = 0; i != MAX_NUM_LIGHTS; ++i) {
		if (i < lights.size()) {
			d_shader.LoadUniform(ArrayName("lightPositions", i), lights[i].position);
			d_shader.LoadUniform(ArrayName("lightColours", i), lights[i].colour);
			d_shader.LoadUniform(ArrayName("lightAttenuations", i), lights[i].attenuation);
		}
		else {  // "Empty" lights to pad the array
			d_shader.LoadUniform(ArrayName("lightPositions", i), {0.0f, 0.0f, 0.0f});
			d_shader.LoadUniform(ArrayName("lightColours", i), {0.0f, 0.0f, 0.0f});
			d_shader.LoadUniform(ArrayName("lightAttenuations", i), {1.0f, 0.0f, 0.0f});
		}
	}
    
    d_shader.Unbind();
}

void TerrainRenderer::Draw(const Terrain& terrain)
{
    RenderContext rc;
    d_shader.Bind();

    // Load up the transform matrix.
	Maths::mat4 transform = Maths::Transform(terrain.Position(), Maths::identity);
    
    d_shader.LoadUniform("transformMatrix", transform);
	d_shader.LoadUniform("shineDamper", terrain.GetMaterial().shineDamper);
	d_shader.LoadUniform("reflectivity", terrain.GetMaterial().reflectivity);
    
    terrain.Bind();
    glDrawElements(GL_TRIANGLES, terrain.GetModel().VertexCount(), GL_UNSIGNED_INT, (const void*)0);
    terrain.Unbind();

    d_shader.Unbind();
}

}