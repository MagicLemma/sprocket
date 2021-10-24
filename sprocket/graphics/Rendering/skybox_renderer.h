#pragma once
#include <sprocket/Graphics/mesh.h>
#include <sprocket/Graphics/shader.h>

#include <glm/glm.hpp>

#include <memory>

namespace spkt {

class cube_map;

class skybox_renderer
{
    spkt::static_mesh d_mesh;
    spkt::shader      d_shader;

public:
    skybox_renderer();

    void draw(const cube_map& skybox, const glm::mat4& proj, const glm::mat4& view);
};

}