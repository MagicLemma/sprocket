#include "Viewport.h"

#include <glad/glad.h>

namespace Sprocket {

Viewport::Viewport(int x, int y, int width, int height)
{
    Set(x, y, width, height);
}

Viewport::Viewport()
{
}

Viewport::~Viewport()
{
    Restore();
}

void Viewport::Set(int x, int y, int width, int height)
{
    d_original = Current();
    glViewport(x, y, width, height);
}

void Viewport::Restore()
{
    if (d_original.has_value()) {
        const auto& original = d_original.value();
        glViewport(original.x, original.y, original.z, original.w);
        d_original = {};
    }
}

glm::ivec4 Viewport::Current()
{
    glm::ivec4 original;
    glGetIntegerv(GL_VIEWPORT, &original.x);
    return original;
}

float Viewport::CurrentAspectRatio()
{
    glm::ivec4 view = Current();
    return (float)view.z/(float)view.w; // width / height
}

}