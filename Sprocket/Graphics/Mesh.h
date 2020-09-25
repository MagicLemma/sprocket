#pragma once
#include "Maths.h"
#include "Resources.h"
#include "BufferLayout.h"

#include <vector>
#include <string>

namespace Sprocket {

struct Vertex
{
    Maths::vec3 position;
    Maths::vec2 textureCoords;

    Maths::vec3 normal;
    Maths::vec3 tangent;
    Maths::vec3 bitangent;
};

struct AnimVertex
{
    Maths::vec3 position;
    Maths::vec2 textureCoords;
    
    Maths::vec3 normal;
    Maths::vec3 tangent;
    Maths::vec3 bitangent;

    Maths::ivec4 boneIndices;
    Maths::vec4  boneWeights;
};

using VertexBuffer = std::vector<Vertex>;
using IndexBuffer = std::vector<unsigned int>;

class Mesh
{
    // GPU STORAGE
    std::shared_ptr<VBO> d_vertexBuffer;
    std::shared_ptr<VBO> d_indexBuffer;

    BufferLayout d_layout; // This describes the layout of a vertex array.

    std::size_t d_count;
    bool d_animated;

public:
    Mesh(const VertexBuffer& vertices, const IndexBuffer& indices);
    Mesh(); // Empty model

    std::size_t VertexCount() const { return d_count; }

    void Bind() const;

    bool operator==(const Mesh& other) const;

    BufferLayout GetLayout() const;
};

}