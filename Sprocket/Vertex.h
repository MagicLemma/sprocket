#pragma once

#include <glad/glad.h>

namespace Sprocket
{

struct Vertex
{
    static constexpr int posAttr = 0;
    static constexpr int colAttr = 1;

    static constexpr int posCount = 3;
    static constexpr int colCount = 3;

    GLfloat position[posCount];
    GLfloat colour[colCount];
};

}