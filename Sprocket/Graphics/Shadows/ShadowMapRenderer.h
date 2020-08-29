#pragma once
#include "Shader.h"
#include "Window.h"
#include "Light.h"
#include "Entity.h"
#include "DepthBuffer.h"
#include "ModelManager.h"
#include "TextureManager.h"
#include "Scene.h"

#include <memory>

namespace Sprocket {

class ShadowMapRenderer
{
    Window*         d_window;
    ModelManager*   d_modelManager;
    TextureManager* d_textureManager;

    Shader  d_shader;

    Maths::mat4 d_lightViewMatrix;
    Maths::mat4 d_lightProjMatrix;

    DepthBuffer d_shadowMap;

public:
    ShadowMapRenderer(Window* window, ModelManager* modelManager);

    void BeginScene(const Sun& sun, const Maths::vec3& centre);
        // Called before any draw calls. The light is the light
        // casting the shadow and the centre is the middle of the
        // shadow box.

    void Draw(const Entity& entity);
        // Draw the entity.

    void EndScene();
        // Called after all draw calls.

    Maths::mat4 GetLightProjViewMatrix() const;
    Texture     GetShadowMap() const;
};

}