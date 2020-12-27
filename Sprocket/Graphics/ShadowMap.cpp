#include "ShadowMap.h"
#include "Components.h"
#include "RenderContext.h"

#include <glad/glad.h>

namespace Sprocket {
namespace {

std::shared_ptr<Buffer> GetInstanceBuffer()
{
    BufferLayout layout(sizeof(InstanceData), 5);
    layout.AddAttribute(DataType::FLOAT, 3, DataRate::INSTANCE);
    layout.AddAttribute(DataType::FLOAT, 4, DataRate::INSTANCE);
    layout.AddAttribute(DataType::FLOAT, 3, DataRate::INSTANCE);
    assert(layout.Validate());

    return std::make_shared<Buffer>(layout, BufferUsage::DYNAMIC);
}

}

ShadowMap::ShadowMap(Window* window, AssetManager* assetManager)
    : d_window(window)
    , d_assetManager(assetManager)
    , d_shader("Resources/Shaders/ShadowMap.vert", "Resources/Shaders/ShadowMap.frag")
    , d_lightViewMatrix() // Will be populated after starting a scene.
    , d_lightProjMatrix(glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, -20.0f, 20.0f))
    , d_shadowMap(window, 8192, 8192)
    , d_vao(std::make_unique<VertexArray>())
    , d_instanceBuffer(GetInstanceBuffer())
{
}

void ShadowMap::Draw(
    const glm::vec3& sunDirection,
    const glm::vec3& centre,
    Scene& scene)
{
    RenderContext rc;
    rc.DepthTesting(true);
    rc.FaceCulling(true);

    d_lightViewMatrix = glm::lookAt(centre - sunDirection, centre, {0.0, 1.0, 0.0});

    d_shader.Bind();
    d_shader.LoadMat4("u_proj_matrix", d_lightProjMatrix);
    d_shader.LoadMat4("u_view_matrix", d_lightViewMatrix);

    d_shadowMap.Bind();
    glEnable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);

    std::unordered_map<std::string, std::vector<InstanceData>> commands;
    for (auto entity : scene.Reg()->View<ModelComponent>()) {
        const auto& tc = entity.Get<TransformComponent>();
        const auto& mc = entity.Get<ModelComponent>();
        if (mc.mesh.empty()) { continue; }
        commands[mc.mesh].push_back({ tc.position, tc.orientation, tc.scale });
    }

    for (const auto& [key, data] : commands) {
        auto mesh = d_assetManager->GetMesh(key);
        if (!mesh) { continue; }

        d_vao->SetModel(mesh);
        d_instanceBuffer->SetData(data);
        d_vao->SetInstances(d_instanceBuffer);
        d_vao->Draw();
    }

    glCullFace(GL_BACK);
    d_shadowMap.Unbind();
    d_shader.Unbind();
}

glm::mat4 ShadowMap::GetLightProjViewMatrix() const
{
    return d_lightProjMatrix * d_lightViewMatrix;
}

std::shared_ptr<Texture> ShadowMap::GetShadowMap() const
{
    return d_shadowMap.GetShadowMap();
}

}