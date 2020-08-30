#pragma once
#include "Skybox.h"
#include "Entity.h"
#include "Shader.h"
#include "VertexArray.h"

#include <memory>

namespace Sprocket {

class SkyboxRenderer
{
    Shader d_shader;
        // Shader used to draw entities.

    std::unique_ptr<VertexArray> d_vao;

public:
    SkyboxRenderer();

    void Draw(const Skybox& skybox, const Entity& camera);
    void Draw(const Skybox& skybox, const Maths::mat4& proj, const Maths::mat4& view);
};

}