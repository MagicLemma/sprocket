#pragma once
#include "Graphics/Entity.h"
#include "Graphics/Camera.h"
#include "Graphics/Shader.h"
#include "Graphics/Light.h"
#include "Core/Window.h"

#include <vector>

namespace Sprocket {

struct RenderOptions
{
    bool faceCulling = true;
};

namespace Renderer {
    
void render(const Entity& entity,
            const std::vector<Light>& lights,
            const Camera& camera,
            const Shader& shader,
            Window* window,
            const RenderOptions& options = RenderOptions());

void render(const Model& model,
            const Shader& shader,
            const RenderOptions& options = RenderOptions());

}
}