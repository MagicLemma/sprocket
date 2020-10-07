#pragma once
#include "Skybox.h"
#include "ModelManager.h"
#include "Entity.h"
#include "Shader.h"
#include "VertexArray.h"

#include <memory>

namespace Sprocket {

class SkyboxRenderer
{
    ModelManager* d_modelManager;

    Shader d_shader;
        // Shader used to draw entities.

    std::unique_ptr<VertexArray> d_vao;

public:
    SkyboxRenderer(ModelManager* modelManager);

    void Draw(const Skybox& skybox, const Entity& camera);
    void Draw(const Skybox& skybox, const Maths::mat4& proj, const Maths::mat4& view);
};

}