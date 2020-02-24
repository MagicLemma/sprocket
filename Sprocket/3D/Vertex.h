#pragma once

#include <vector>

#include <glad/glad.h>

namespace Sprocket {

struct Vertex
{
    static constexpr int posAttr = 0;
    static constexpr int texAttr = 1;
    static constexpr int norAttr = 2;

    static constexpr int posCount = 3;
    static constexpr int texCount = 2;
    static constexpr int norCount = 3;

    GLfloat position[posCount];
    GLfloat texture[texCount];
    GLfloat normal[norCount];
};

using VertexBuffer = std::vector<Vertex>;
using IndexBuffer = std::vector<unsigned int>;

}