#include "viewport.h"

#include <glad/glad.h>

namespace spkt {

viewport::viewport(int x, int y, int width, int height)
{
    set(x, y, width, height);
}

viewport::viewport()
{
}

viewport::~viewport()
{
    restore();
}

void viewport::set(int x, int y, int width, int height)
{
    d_original = current_viewport();
    glViewport(x, y, width, height);
}

void viewport::restore()
{
    if (d_original.has_value()) {
        const auto& original = d_original.value();
        glViewport(original.x, original.y, original.z, original.w);
        d_original = {};
    }
}

glm::ivec4 viewport::current_viewport()
{
    glm::ivec4 original;
    glGetIntegerv(GL_VIEWPORT, &original.x);
    return original;
}

float viewport::current_aspect_ratio()
{
    glm::ivec4 view = current_viewport();
    return (float)view.z/(float)view.w; // width / height
}

}