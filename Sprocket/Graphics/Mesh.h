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

using VertexBuffer = std::vector<Vertex>;
using IndexBuffer = std::vector<unsigned int>;

class Mesh
{
    std::shared_ptr<VBO> d_vertexBuffer;
    std::shared_ptr<VBO> d_indexBuffer;

    BufferLayout d_layout;
    std::size_t d_vertexCount;

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

public:
    Mesh(const VertexBuffer& vertices, const IndexBuffer& indices);
    Mesh(); // Empty model

    static std::shared_ptr<Mesh> FromFile(const std::string& file);

    // Core Functionality
    std::size_t VertexCount() const { return d_vertexCount; }
    BufferLayout GetLayout() const;
    void Bind() const;

    bool operator==(const Mesh& other) const;
};

}