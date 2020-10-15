#pragma once
#include "Maths.h"
#include "Resources.h"
#include "BufferLayout.h"
#include "Animation.h"

#include <vector>
#include <string>
#include <memory>

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

    Maths::ivec4 boneIndices = {-1, -1, -1, -1};
    Maths::vec4  boneWeights = {0.0, 0.0, 0.0, 0.0};
};

using VertexBuffer = std::vector<Vertex>;
using AnimVertexBuffer = std::vector<AnimVertex>;
using IndexBuffer = std::vector<std::uint32_t>;

struct StaticMeshData
{
    VertexBuffer vertices;
    IndexBuffer  indices;
};

struct AnimatedMeshData
{
    AnimVertexBuffer vertices;
    IndexBuffer      indices;

    Skeleton skeleton;
    std::unordered_map<std::string, Animation> animations;
};

class Mesh
{
    std::uint32_t d_vertexBuffer;
    std::uint32_t d_indexBuffer;

    BufferLayout d_layout;
    std::size_t d_vertexCount;

    // Animation data structures. If this meshes is not animated, these
    // structures are empty
    bool d_animated;
    Skeleton d_skeleton;
    std::vector<Maths::mat4> d_currentPose;

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

public:
    Mesh(const StaticMeshData& data);
    Mesh(const AnimatedMeshData& data);
    Mesh(); // Empty model
    ~Mesh();

    static std::shared_ptr<Mesh> FromFile(const std::string& file);

    // Core Functionality
    std::size_t VertexCount() const { return d_vertexCount; }
    BufferLayout GetLayout() const;
    void Bind() const;

    bool operator==(const Mesh& other) const;

    // Animation Functionality
    bool IsAnimated() const { return d_animated; }

    const std::vector<Maths::mat4>& SetPose(const std::string& name, float time);
        // Returns the transforms to be uploaded to the shader. The transform
        // at position i corresponds to the bone with ID i.

    std::vector<std::string> GetAnimationNames() const;
        // Returns a list of names of all possible animations in this mesh.
};

}