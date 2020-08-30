#pragma once
#include "Shader.h"
#include "Window.h"
#include "Light.h"
#include "Entity.h"
#include "DepthBuffer.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Scene.h"
#include "VertexArray.h"

#include <memory>

namespace Sprocket {

class ShadowMap
{
    Window*         d_window;
    ModelManager*   d_modelManager;
    TextureManager* d_textureManager;

    Shader  d_shader;

    Maths::mat4 d_lightViewMatrix;
    Maths::mat4 d_lightProjMatrix;

    DepthBuffer d_shadowMap;

    std::unique_ptr<VertexArray> d_vao;
    std::shared_ptr<InstanceBuffer> d_instanceBuffer;

public:
    ShadowMap(Window* window, ModelManager* modelManager);

    void Draw(const Sun& sun, const Maths::vec3& centre, Scene& scene);

    Maths::mat4 GetLightProjViewMatrix() const;
    Texture     GetShadowMap() const;
};

}