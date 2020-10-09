#pragma once
#include "Shader.h"
#include "Window.h"

#include "Entity.h"
#include "DepthBuffer.h"
#include "AssetManager.h"
#include "Scene.h"
#include "VertexArray.h"

#include <memory>

namespace Sprocket {

class ShadowMap
{
    Window*         d_window;
    AssetManager*   d_assetManager;

    Shader  d_shader;

    Maths::mat4 d_lightViewMatrix;
    Maths::mat4 d_lightProjMatrix;

    DepthBuffer d_shadowMap;

    std::unique_ptr<VertexArray> d_vao;
    std::shared_ptr<Buffer> d_instanceBuffer;

    std::vector<InstanceData> d_instanceData;

public:
    ShadowMap(Window* window, AssetManager* assetManager);

    void Draw(const Sun& sun, const Maths::vec3& centre, Scene& scene);

    Maths::mat4 GetLightProjViewMatrix() const;
    Texture     GetShadowMap() const;
};

}