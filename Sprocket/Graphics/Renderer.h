#pragma once
#include "Graphics/Entity.h"
#include "Graphics/Camera.h"
#include "Graphics/Shader.h"
#include "Graphics/Light.h"
#include "Graphics/GUI/Model2D.h"

#include <vector>

namespace Sprocket {
namespace Renderer {
    
void render(const Entity& entity,
            const std::vector<Light>& lights,
            const Camera& camera,
            const Shader& shader);

void render(const Model2D& model,
            const Shader& shader);

}
}