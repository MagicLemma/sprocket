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

struct Vertex2D
{
    static constexpr int posAttr = 0;
    
    static constexpr int posCount = 2;

    GLfloat position[posCount];
};

struct VertexSkybox
{
    static constexpr int posAttr = 0;
    
    static constexpr int posCount = 3;

    GLfloat position[posCount];

};

using VertexBuffer = std::vector<Vertex>;
using IndexBuffer = std::vector<unsigned int>;

using Vertex2DBuffer = std::vector<Vertex2D>;

using VertexSkyboxBuffer = std::vector<VertexSkybox>;

}