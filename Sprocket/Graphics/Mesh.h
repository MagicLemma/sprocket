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
using AnimVertexBuffer = std::vector<AnimVertex>;
using IndexBuffer = std::vector<unsigned int>;

class Mesh
{
    std::shared_ptr<VBO> d_vertexBuffer;
    std::shared_ptr<VBO> d_indexBuffer;

    BufferLayout d_layout;
    std::size_t d_vertexCount;

    // Animated Data
    bool d_animated;
    std::unordered_map<std::string, std::uint32_t> m_BoneMap;

public:
    Mesh(const VertexBuffer& vertices, const IndexBuffer& indices);
    Mesh(const AnimVertexBuffer& vertices, const IndexBuffer& indices);
    Mesh(); // Empty model

    // Core Functionality
    std::size_t VertexCount() const { return d_vertexCount; }
    BufferLayout GetLayout() const;
    void Bind() const;

    // Animated Functionality
    bool IsAnimated() const { return d_animated; }

    bool operator==(const Mesh& other) const;
};

}