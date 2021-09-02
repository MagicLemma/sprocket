#include "ShadowMap.h"

#include <Sprocket/Graphics/render_context.h>
#include <Sprocket/Scene/ecs.h>

#include <glad/glad.h>

namespace spkt {

ShadowMap::ShadowMap(AssetManager* assetManager)
    : d_assetManager(assetManager)
    , d_shader("Resources/Shaders/ShadowMap.vert", "Resources/Shaders/ShadowMap.frag")
    , d_lightViewMatrix() // Will be populated after starting a scene.
    , d_lightProjMatrix(glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, -50.0f, 50.0f))
    , d_shadowMap(8192, 8192)
{
}

void ShadowMap::Draw(
    spkt::registry& registry,
    const glm::vec3& sunDirection,
    const glm::vec3& centre)
{
    spkt::render_context rc;
    rc.depth_testing(true);

    // Reduces "peter panning", also stops "flat" objects that dont have a back from
    // casting shadows.
    rc.set_face_cull(GL_FRONT);

    d_lightViewMatrix = glm::lookAt(centre - sunDirection, centre, {0.0, 1.0, 0.0});

    d_shader.bind();
    d_shader.load("u_proj_matrix", d_lightProjMatrix);
    d_shader.load("u_view_matrix", d_lightViewMatrix);

    d_shadowMap.bind();
    glClear(GL_DEPTH_BUFFER_BIT);

    std::unordered_map<std::string, std::vector<model_instance>> commands;
    for (auto [mc, tc] : registry.view_get<StaticModelComponent, Transform3DComponent>()) {
        commands[mc.mesh].push_back({ tc.position, tc.orientation, tc.scale });
    }

    for (const auto& [key, data] : commands) {
        d_instance_buffer.set_data(data);
        spkt::draw(d_assetManager->get_static_mesh(key), &d_instance_buffer);
    }

    d_shadowMap.unbind();
    d_shader.unbind();
}

glm::mat4 ShadowMap::GetLightProjViewMatrix() const
{
    return d_lightProjMatrix * d_lightViewMatrix;
}

Texture* ShadowMap::GetShadowMap() const
{
    return d_shadowMap.depth_texture();
}

}