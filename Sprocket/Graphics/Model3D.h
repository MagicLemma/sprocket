#pragma once
#include "Maths.h"
#include "Resources.h"
#include "BufferLayout.h"

#include <vector>
#include <string>

namespace Sprocket {

struct Vertex3D
{
    Maths::vec3 position;
    Maths::vec3 normal;
    Maths::vec3 tangent;
    Maths::vec3 bitangent;
    Maths::vec2 textureCoords;
};

using Vertex3DBuffer = std::vector<Vertex3D>;
using IndexBuffer = std::vector<unsigned int>;

class Mesh
{
    // GPU STORAGE
    std::shared_ptr<VBO> d_vertexBuffer;
    std::shared_ptr<VBO> d_indexBuffer;

    BufferLayout d_layout; // This describes the layout of a Vertex3D array.

    std::size_t d_count;

public:
    Mesh(const Vertex3DBuffer& vertices, const IndexBuffer& indices);
    Mesh(); // Empty model

    std::size_t VertexCount() const { return d_count; }

    void Bind() const;

    bool operator==(const Mesh& other) const;

    BufferLayout GetLayout() const;
};

}