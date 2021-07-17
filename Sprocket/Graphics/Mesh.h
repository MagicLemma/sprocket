#pragma once
#include <Sprocket/Graphics/BufferLayout.h>
#include <Sprocket/Graphics/Animation.h>

#include <glm/glm.hpp>

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace spkt {

struct Vertex
{
    glm::vec3 position;
    glm::vec2 textureCoords;

    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct AnimVertex
{
    glm::vec3 position;
    glm::vec2 textureCoords;

    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    glm::ivec4 boneIndices = {-1, -1, -1, -1};
    glm::vec4  boneWeights = {0.0, 0.0, 0.0, 0.0};
};

struct StaticMeshData
{
    std::vector<Vertex>        vertices;
    std::vector<std::uint32_t> indices;
};

struct AnimatedMeshData
{
    std::vector<AnimVertex>    vertices;
    std::vector<std::uint32_t> indices;

    Skeleton skeleton;
};

struct MeshData
{
    std::variant<StaticMeshData, AnimatedMeshData> data;

    MeshData(const MeshData&) = delete;
    MeshData& operator=(const MeshData&) = delete;

    MeshData(const std::string& file);
};

class Mesh
{
    std::uint32_t d_vertexBuffer;
    std::uint32_t d_indexBuffer;
    std::uint64_t d_vertexCount;

    BufferLayout d_layout;

    std::optional<Skeleton> d_skeleton;

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

public:
    Mesh(const StaticMeshData& data);
    Mesh(const AnimatedMeshData& data);

    Mesh(); // Empty model
    ~Mesh();

    static std::unique_ptr<Mesh> FromData(const MeshData& data);
    static std::unique_ptr<Mesh> FromFile(const std::string& file);

    std::size_t VertexCount() const { return d_vertexCount; }
    BufferLayout GetLayout() const;
    void Bind() const;

    // Returns true if this Mesh contains animation data.
    bool IsAnimated() const { return d_skeleton.has_value(); }

    // Returns the transforms to be uploaded to the shader. The transform
    // at position i corresponds to the bone with ID i.
    std::vector<glm::mat4> GetPose(const std::string& name, float time) const;

    // Returns a list of names of all possible animations in this mesh.
    std::vector<std::string> GetAnimationNames() const;
};

}