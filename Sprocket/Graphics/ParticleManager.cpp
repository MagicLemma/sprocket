#include "ParticleManager.h"
#include "ModelManager.h"
#include "RenderContext.h"
#include "Components.h"
#include "Light.h"
#include "Random.h"
#include "CameraUtils.h"

namespace Sprocket {
    
ParticleManager::ParticleManager()
    : d_vao(std::make_unique<VertexArray>())
    , d_shader("Resources/Shaders/Entity.vert", "Resources/Shaders/Entity.frag")
    , d_model(ModelManager::LoadModel("Resources/Models/Particle.obj"))
    , d_instances(std::make_shared<InstanceBuffer>(NUM_PARTICLES))
{
    d_vao->SetModel(d_model);
}

void ParticleManager::Emit(const Particle& particle)
{
    auto& p = d_particles[d_index];
    p.position = particle.position;
    p.velocity = particle.velocity;
    p.scale = particle.scale;
    p.life = particle.life;
    d_index = --d_index % NUM_PARTICLES;
}

void ParticleManager::Draw(double dt, const Entity& camera, const Lights& lights, Scene& scene)
{
    Maths::mat4 proj = CameraUtils::MakeProj(camera);
    Maths::mat4 view = CameraUtils::MakeView(camera);
    Draw(dt, proj, view, lights, scene);
}

void ParticleManager::Draw(double dt, const Maths::mat4& proj, const Maths::mat4& view, const Lights& lights, Scene& scene)
{
    RenderContext rc;
    rc.FaceCulling(true);
    rc.DepthTesting(true);

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


    d_instances->Clear();

    for (auto& particle : d_particles) {
        particle.life -= dt;
        particle.position += particle.velocity * (float)dt;
        particle.velocity += particle.acceleration * (float)dt;

        float r = Random<float>(1.0f, 3.0f);
        if (particle.life > 0.0) {
            d_instances->Add({
                particle.position,
                {0.0, 0.0, 0.0, 1.0},
                particle.scale,
                1.0,
                1.0
            });
        }
    }

    d_shader.Bind();
    d_vao->SetInstances(d_instances);
    d_vao->Draw();
}

}