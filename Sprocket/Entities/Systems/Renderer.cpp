#include "Renderer.h"
#include "EntityManager.h"
#include "CameraUtils.h"
#include "RenderContext.h"

#include <glad/glad.h>

namespace Sprocket {

Renderer::Renderer(Window* window,
                   ModelManager* modelManager,
                   TextureManager* textureManager)
    : d_window(window)
    , d_modelManager(modelManager)
    , d_textureManager(textureManager)
    , d_shader("Resources/Shaders/Entity.vert",
               "Resources/Shaders/Entity.frag")
    , d_shadowMap(window, modelManager, textureManager)
{
}

void Renderer::OnUpdate(EntityManager& manager, double dt)
{
    if (d_camera == Entity()) { return; } // No camera

    if (d_renderShadows) {
        float lambda = 5.0f; // TODO: Calculate the floor intersection point
        Maths::vec3 target = d_camera.Get<TransformComponent>().position + lambda * Maths::Forwards(d_camera.Get<TransformComponent>().orientation);
        d_shadowMap.BeginScene(d_lights.sun, target);
        manager.Each<TransformComponent, ModelComponent>([&](Entity& entity) {
            d_shadowMap.Draw(entity);
        });
        d_shadowMap.EndScene();
    }

    Maths::mat4 view = CameraUtils::MakeView(d_camera);
    Maths::mat4 proj = CameraUtils::MakeProj(d_camera);

    unsigned int MAX_NUM_LIGHTS = 5;

    d_shader.Bind();
    d_shader.LoadUniform("u_proj_matrix", proj);
    d_shader.LoadUniform("u_view_matrix", view);

    // Load sun to shader
    d_shader.LoadUniform("u_sun_direction", d_lights.sun.direction);
    d_shader.LoadUniform("u_sun_colour", d_lights.sun.colour);
    d_shader.LoadUniform("u_sun_brightness", d_lights.sun.brightness);

    // Load ambience to shader
    d_shader.LoadUniform("u_ambience_colour", d_lights.ambience.colour);
    d_shader.LoadUniform("u_ambience_brightness", d_lights.ambience.brightness);
    
    // Load point lights to shader
    for (size_t i = 0; i != MAX_NUM_LIGHTS; ++i) {
		if (i < d_lights.points.size()) {
			d_shader.LoadUniform(ArrayName("u_light_pos", i), d_lights.points[i].position);
			d_shader.LoadUniform(ArrayName("u_light_colour", i), d_lights.points[i].colour);
			d_shader.LoadUniform(ArrayName("u_light_attenuation", i), d_lights.points[i].attenuation);
		}
		else {  // "Empty" lights to pad the array
			d_shader.LoadUniform(ArrayName("u_light_pos", i), {0.0f, 0.0f, 0.0f});
			d_shader.LoadUniform(ArrayName("u_light_colour", i), {0.0f, 0.0f, 0.0f});
			d_shader.LoadUniform(ArrayName("u_light_attenuation", i), {1.0f, 0.0f, 0.0f});
		}
	}

    if (d_renderShadows) {
        glActiveTexture(GL_TEXTURE3);
        d_shadowMap.GetShadowMap().Bind();
        d_shader.LoadUniformInt("shadow_map", 3);
        d_shader.LoadUniform("u_light_proj_view", d_shadowMap.GetLightProjViewMatrix());
        glActiveTexture(GL_TEXTURE0);
    }

    RenderContext rc;
    rc.FaceCulling(true);
    rc.DepthTesting(true);

    manager.Each<TransformComponent, ModelComponent>([&](Entity& entity) {

        auto& modelComp = entity.Get<ModelComponent>();
        auto tr = entity.Get<TransformComponent>();
        Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation);
        transform = Maths::Scale(transform, modelComp.scale);

        d_shader.LoadUniform("u_model_matrix", transform);
        d_shader.LoadUniform("u_shine_dampner", modelComp.shineDamper);
        d_shader.LoadUniform("u_reflectivity", modelComp.reflectivity);

        float brightness = 1.0f;
        if (entity.Has<SelectComponent>()) {
            auto data = entity.Get<SelectComponent>();
            if (data.selected) {
                brightness = 1.5f;
            } else if (data.hovered) {
                brightness = 1.25f;
            }
        }
        d_shader.LoadUniform("u_brightness", brightness);

        auto model = d_modelManager->GetModel(modelComp.model);
        auto texture = d_textureManager->GetTexture(modelComp.texture);

        glActiveTexture(GL_TEXTURE0);
        texture.Bind();
        model.Bind();
        glDrawElements(GL_TRIANGLES, (int)model.VertexCount(), GL_UNSIGNED_INT, nullptr);
        model.Unbind();
        texture.Unbind();
    });

    d_shader.Unbind();
};

}