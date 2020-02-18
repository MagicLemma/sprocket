#pragma once

#include <glad/glad.h>

namespace Sprocket
{

struct Vertex
{
    static constexpr int posAttr = 0;
    static constexpr int texAttr = 1;

    static constexpr int posCount = 3;
    static constexpr int texCount = 2;

    GLfloat position[posCount];
    GLfloat texture[texCount];
};

}