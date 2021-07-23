#include "ShadowMap.h"

#include <Sprocket/Graphics/RenderContext.h>
#include <Sprocket/Scene/ecs.h>

#include <glad/glad.h>

namespace spkt {
namespace {

std::unique_ptr<Buffer> GetInstanceBuffer()
{
    BufferLayout layout(sizeof(InstanceData), 5);
    layout.AddAttribute(DataType::FLOAT, 3, DataRate::INSTANCE);
    layout.AddAttribute(DataType::FLOAT, 4, DataRate::INSTANCE);
    layout.AddAttribute(DataType::FLOAT, 3, DataRate::INSTANCE);
    assert(layout.Validate());

    return std::make_unique<Buffer>(layout, BufferUsage::DYNAMIC);
}

}

ShadowMap::ShadowMap(AssetManager* assetManager)
    : d_assetManager(assetManager)
    , d_shader("Resources/Shaders/ShadowMap.vert", "Resources/Shaders/ShadowMap.frag")
    , d_lightViewMatrix() // Will be populated after starting a scene.
    , d_lightProjMatrix(glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, -50.0f, 50.0f))
    , d_shadowMap(8192, 8192)
    , d_vao(std::make_unique<VertexArray>())
    , d_instanceBuffer(GetInstanceBuffer())
{
}

void ShadowMap::Draw(
    spkt::registry& registry,
    const glm::vec3& sunDirection,
    const glm::vec3& centre)
{
    spkt::RenderContext rc;
    rc.DepthTesting(true);
    rc.FaceCulling(true);

    d_lightViewMatrix = glm::lookAt(centre - sunDirection, centre, {0.0, 1.0, 0.0});

    d_shader.bind();
    d_shader.load("u_proj_matrix", d_lightProjMatrix);
    d_shader.load("u_view_matrix", d_lightViewMatrix);

    d_shadowMap.Bind();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Reduces "peter panning", also stops "flat" objects that dont have a back from
    // casting shadows.
    glCullFace(GL_FRONT);

    std::unordered_map<std::string, std::vector<InstanceData>> commands;
    for (auto entity : registry.view<ModelComponent, Transform3DComponent>()) {
        const auto& tc = registry.get<Transform3DComponent>(entity);
        const auto& mc = registry.get<ModelComponent>(entity);
        if (mc.mesh.empty()) { continue; }
        commands[mc.mesh].push_back({ tc.position, tc.orientation, tc.scale });
    }

    for (const auto& [key, data] : commands) {
        auto mesh = d_assetManager->GetMesh(key);
        if (!mesh) { continue; }

        d_vao->SetModel(mesh);
        d_instanceBuffer->SetData(data);
        d_vao->SetInstances(d_instanceBuffer.get());
        d_vao->Draw();
    }

    glCullFace(GL_BACK);
    d_shadowMap.Unbind();
    d_shader.unbind();
}

glm::mat4 ShadowMap::GetLightProjViewMatrix() const
{
    return d_lightProjMatrix * d_lightViewMatrix;
}

Texture* ShadowMap::GetShadowMap() const
{
    return d_shadowMap.GetShadowMap();
}

}