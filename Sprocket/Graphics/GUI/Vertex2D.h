#pragma once
#include <vector>
#include <glad/glad.h>

namespace Sprocket {

struct Vertex2D
{
    static constexpr int posAttr = 0;
    static constexpr int posCount = 2;

    GLfloat position[posCount];
};

using Vertex2DBuffer = std::vector<Vertex2D>;

}