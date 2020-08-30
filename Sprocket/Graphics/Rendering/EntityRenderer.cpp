#include "EntityRenderer.h"
#include "Maths.h"
#include "ModelManager.h"
#include "RenderContext.h"
#include "CameraUtils.h"
#include "Components.h"
#include "Scene.h"

#include <glad/glad.h>

namespace Sprocket {

EntityRenderer::EntityRenderer(Window* window,
                               ModelManager* modelManager,
                               TextureManager* textureManager)
    : d_vao(std::make_unique<VertexArray>())
    , d_window(window)
    , d_modelManager(modelManager)
    , d_textureManager(textureManager)
    , d_shader("Resources/Shaders/Entity.vert", "Resources/Shaders/Entity.frag")
{
}

void EntityRenderer::EnableShadows(
    const Texture& shadowMap,
    const Maths::mat4& lightProjView)
{
    glActiveTexture(GL_TEXTURE3);
    shadowMap.Bind();
 
    d_shader.Bind();
    d_shader.LoadUniformInt("shadow_map", 3);
    d_shader.LoadUniform("u_light_proj_view", lightProjView);
    glActiveTexture(GL_TEXTURE0);
}

void EntityRenderer::Draw(
    const Maths::mat4& proj,
    const Maths::mat4& view,
    const Lights& lights,
    Scene& scene)
{
    RenderContext rc;
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_DEPTH_TEST);

    unsigned int MAX_NUM_LIGHTS = 5;

    d_shader.Bind();
    d_shader.LoadUniform("u_proj_matrix", proj);
    d_shader.LoadUniform("u_view_matrix", view);

    // Load sun to shader
    const auto& sun = scene.GetSun();
    d_shader.LoadUniform("u_sun_direction", sun.direction);
    d_shader.LoadUniform("u_sun_colour", sun.colour);
    d_shader.LoadUniform("u_sun_brightness", sun.brightness);

    // Load ambience to shader
    d_shader.LoadUniform("u_ambience_colour", lights.ambience.colour);
    d_shader.LoadUniform("u_ambience_brightness", lights.ambience.brightness);
    
    // Load point lights to shader
    std::size_t i = 0;
    scene.Each<TransformComponent, LightComponent>([&](Entity& entity) {
        if (i < MAX_NUM_LIGHTS) {
            auto position = entity.Get<TransformComponent>().position;
            auto light = entity.Get<LightComponent>();
            d_shader.LoadUniform(ArrayName("u_light_pos", i), position);
			d_shader.LoadUniform(ArrayName("u_light_colour", i), light.colour);
			d_shader.LoadUniform(ArrayName("u_light_attenuation", i), light.attenuation);
            d_shader.LoadUniform(ArrayName("u_light_brightness", i), light.brightness);
            ++i;
        }
    });
    while (i < MAX_NUM_LIGHTS) {
        d_shader.LoadUniform(ArrayName("u_light_pos", i), {0.0f, 0.0f, 0.0f});
        d_shader.LoadUniform(ArrayName("u_light_colour", i), {0.0f, 0.0f, 0.0f});
        d_shader.LoadUniform(ArrayName("u_light_attenuation", i), {1.0f, 0.0f, 0.0f});
        d_shader.LoadUniform(ArrayName("u_light_brightness", i), 0.0f);
        ++i;
    }

    scene.Each<TransformComponent, ModelComponent>([&](Entity& entity) {
        DrawModel(entity);
    });

    d_shader.Unbind();
}

void EntityRenderer::Draw(const Entity& camera, const Lights& lights, Scene& scene)
{
    Maths::mat4 proj = CameraUtils::MakeProj(camera);
    Maths::mat4 view = CameraUtils::MakeView(camera);
    Draw(proj, view, lights, scene);
}

void EntityRenderer::DrawModel(const Entity& entity)
{
    const auto tr = entity.Get<TransformComponent>();
    const auto& modelComp = entity.Get<ModelComponent>();
    if (modelComp.model.empty()) { return; }

    Maths::mat4 transform = Maths::Transform(tr.position, tr.orientation, tr.scale);

    d_shader.Bind();
    d_shader.LoadUniform("u_model_matrix", transform);
	d_shader.LoadUniform("u_shine_dampner", modelComp.shineDamper);
	d_shader.LoadUniform("u_reflectivity", modelComp.reflectivity);

    auto model = d_modelManager->GetModel(modelComp.model);
    auto texture = d_textureManager->GetTexture(modelComp.texture);

    glActiveTexture(GL_TEXTURE0);
    texture.Bind();

    d_vao->SetModel(model);
    d_vao->Draw();

    texture.Unbind();
}

}