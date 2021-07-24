#include "Mesh.h"

#include <Sprocket/Utility/Maths.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glad/glad.h>

#include <cassert>
#include <ranges>

namespace spkt {
namespace {

glm::mat4 Convert(const aiMatrix4x4& matrix)
{
    glm::mat4 result;

    result[0][0] = matrix.a1;
    result[0][1] = matrix.b1;
    result[0][2] = matrix.c1;
    result[0][3] = matrix.d1;

    result[1][0] = matrix.a2;
    result[1][1] = matrix.b2;
    result[1][2] = matrix.c2;
    result[1][3] = matrix.d2;

    result[2][0] = matrix.a3;
    result[2][1] = matrix.b3;
    result[2][2] = matrix.c3;
    result[2][3] = matrix.d3;

    result[3][0] = matrix.a4;
    result[3][1] = matrix.b4;
    result[3][2] = matrix.c4;
    result[3][3] = matrix.d4;

    return result;
}

glm::vec2 Convert(const aiVector2D& v)
{
    return glm::vec2{v.x, v.y};
}

glm::vec3 Convert(const aiVector3D& v)
{
    return glm::vec3{v.x, v.y, v.z};
}

glm::quat Convert(const aiQuaternion& q)
{
    glm::quat quat;
    quat.x = q.x;
    quat.y = q.y;
    quat.z = q.z;
    quat.w = q.w;
    return quat;
}

bool IsSceneValid(const aiScene* scene)
    // Returns true if the scene is valid and false otherwise.
{
    return scene && 
           !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) &&
           scene->mRootNode &&
           scene->mNumMeshes > 0;
}

int GetAssimpFlags()
{
    return aiProcess_Triangulate
         | aiProcess_FlipUVs
         | aiProcess_CalcTangentSpace
         | aiProcess_GenUVCoords
         | aiProcess_GenNormals
         | aiProcess_ValidateDataStructure;
}

void AddBoneData(animated_vertex& vertex, std::uint32_t index, float weight)
{
    for (std::uint32_t i = 0; i != 4; ++i) {
        if (vertex.boneIndices[i] == -1) {
            vertex.boneIndices[i] = index;
            vertex.boneWeights[i] = weight;
            return;
        }
    }
    
    // Already got four acting bones, so check if any are less influential
    // than this one and replace it if so.
    for (std::uint32_t i = 0; i != 4; ++i) {
        if (vertex.boneWeights[i] < weight) {
            vertex.boneIndices[i] = index;
            vertex.boneWeights[i] = weight;
            return;
        }
    }
}

aiNodeAnim* GetNodeAnim(aiAnimation* animation, const std::string& name)
{
    for (std::uint32_t i = 0; i != animation->mNumChannels; ++i) {
        aiNodeAnim* node = animation->mChannels[i];
        if (name == std::string(node->mNodeName.data)) {
            return node;
        }
    }
    assert(false);
    return nullptr;
}

std::string NodeName(const aiNode* node)
{
    if (!node) { return ""; }
    return std::string(node->mName.data);
}

Bone* GetBone(Skeleton& skeleton, const aiNode* node)
{
    if (!node) { return nullptr; }
    auto it = skeleton.boneMap.find(NodeName(node));
    if (it != skeleton.boneMap.end()) {
        return &skeleton.bones[it->second];
    }
    return nullptr;
}

bool IsBone(const Skeleton& skeleton, const aiNode* node)
{
    if (!node) { return false; }
    auto it = skeleton.boneMap.find(NodeName(node));
    return it != skeleton.boneMap.end();
}

void LoadAnimations(Skeleton& skeleton, Bone* bone, const aiScene* scene, const glm::mat4& transform)
{
    assert(bone);
    for (std::uint32_t i = 0; i != scene->mNumAnimations; ++i) {
        aiAnimation* animationData = scene->mAnimations[i];
        float ticksPerSec = animationData->mTicksPerSecond;
        if (ticksPerSec == 0.0f) { ticksPerSec = 25.0f; } // If unknown

        const aiNodeAnim* keyFrames = GetNodeAnim(animationData, bone->name);
        std::string name(animationData->mName.data);

        Animation& animation = skeleton.animations[name];
        animation.duration = animationData->mDuration / ticksPerSec;
        BoneKeyFrames& keyFrameData = animation.keyFrames[bone->index];
        for (std::uint32_t i = 0; i != keyFrames->mNumPositionKeys; ++i) {
            auto& x = keyFrames->mPositionKeys[i];
            keyFrameData.keyPostitions.push_back({
                (float)x.mTime / ticksPerSec,
                Maths::ApplyTransform(transform, Convert(x.mValue))
            });
        }

        for (std::uint32_t i = 0; i != keyFrames->mNumRotationKeys; ++i) {
            auto& x = keyFrames->mRotationKeys[i];
            keyFrameData.keyOrientations.push_back({
                (float)x.mTime / ticksPerSec,
                glm::quat_cast(transform) * Convert(x.mValue)
            });
        }

        for (std::uint32_t i = 0; i != keyFrames->mNumScalingKeys; ++i) {
            auto& x = keyFrames->mScalingKeys[i];
            keyFrameData.keyScales.push_back({
                (float)x.mTime/ticksPerSec,
                Convert(x.mValue)
            });
        }
    }

}

// The recursive function for loading skeleton and animation data.
void LoadSkeleton(
    Skeleton& skeleton,
    const aiScene* scene,
    const aiNode* lastBoneNode, // The last node that contained a bone, starts out null
    const aiNode* currentNode,  // The current node that we are dealing with.
    const glm::mat4& parentTransform
)
// NOTE: The parentTransform is an artifact of Assimp's node tree. There may be nodes that don't
// correspond to bones but their transform matrices DO affect bones that are child nodes of it.
// In this algorithm, we keep track of this transform, and when we reach a bone, we apply the
// transform to all of the key frame data, and then pass the identity to its children. In
// essence, we "bake" the intermediate transforms into the animation data so that we do not need
// non-bone nodes in our skeleton tree.
{
    assert(currentNode);

    glm::mat4 nodeTransform = parentTransform * Convert(currentNode->mTransformation);
    if (IsBone(skeleton, currentNode)) {
        // Reset the nodeTransform; the previous will get baked into this bones' animations.
        nodeTransform = glm::mat4(1.0);
        lastBoneNode = currentNode;

        Bone* bone = GetBone(skeleton, lastBoneNode);
        LoadAnimations(skeleton, bone, scene, Maths::NoScale(parentTransform));
    }

    Bone* currentBone = GetBone(skeleton, lastBoneNode);
    for (std::uint32_t i = 0; i != currentNode->mNumChildren; ++i) {
        aiNode* childNode = currentNode->mChildren[i];

        Bone* childBone = GetBone(skeleton, childNode);
        if (childBone && currentBone) {
            currentBone->children.push_back(childBone->index);
        }

        LoadSkeleton(skeleton, scene, lastBoneNode, childNode, nodeTransform);
    }
}

}

static_mesh_data static_mesh_data::load(const std::string& file)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file, GetAssimpFlags());
    assert(IsSceneValid(scene));
    static_mesh_data data;

    for (std::uint32_t idx = 0; idx != scene->mNumMeshes; ++idx) {
        aiMesh* mesh = scene->mMeshes[idx];

        // Vertices
        for (std::uint32_t i = 0; i != mesh->mNumVertices; ++i) {
            static_vertex vertex;
            vertex.position = Convert(mesh->mVertices[i]);
            vertex.normal = Convert(mesh->mNormals[i]);
            vertex.textureCoords = Convert(mesh->mTextureCoords[0][i]);
            vertex.tangent = Convert(mesh->mTangents[i]);
            vertex.bitangent = Convert(mesh->mBitangents[i]);
            data.vertices.push_back(vertex);
        }

        // Indices
        for (std::uint32_t i = 0; i != mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (std::uint32_t j = 0; j != face.mNumIndices; ++j) {
                data.indices.push_back(face.mIndices[j]);
            }
        }
    }

    return data;
}

animated_mesh_data animated_mesh_data::load(const std::string& file)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file, GetAssimpFlags());
    assert(IsSceneValid(scene));
    animated_mesh_data data;

    std::uint32_t vertexCount = 0;
    for (std::uint32_t idx = 0; idx != scene->mNumMeshes; ++idx) {
        aiMesh* mesh = scene->mMeshes[idx];

        // Vertices
        for (std::uint32_t i = 0; i != mesh->mNumVertices; ++i) {
            animated_vertex vertex;
            vertex.position = Convert(mesh->mVertices[i]);
            vertex.normal = Convert(mesh->mNormals[i]);
            vertex.textureCoords = Convert(mesh->mTextureCoords[0][i]);
            vertex.tangent = Convert(mesh->mTangents[i]);
            vertex.bitangent = Convert(mesh->mBitangents[i]);
            data.vertices.push_back(vertex);
        }

        // Indices
        for (std::uint32_t i = 0; i != mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (std::uint32_t j = 0; j != face.mNumIndices; ++j) {
                data.indices.push_back(vertexCount + face.mIndices[j]);
            }
        }

        // Bones
        for (std::uint32_t i = 0; i != mesh->mNumBones; ++i) {
            aiBone* bone = mesh->mBones[i];
            std::string boneName(bone->mName.data);

            // We have to do this lookup as a bone be shared by multiple
            // submeshes, so we may have already encountered this bone.
            std::uint32_t boneIndex = 0;
            auto it = data.skeleton.boneMap.find(boneName);
            if (it != data.skeleton.boneMap.end()) {
                boneIndex = data.skeleton.boneMap[boneName];
            } else {
                boneIndex = data.skeleton.bones.size();
                data.skeleton.boneMap[boneName] = boneIndex;
                
                Bone newBone;
                newBone.name = boneName;
                newBone.index = boneIndex;
                newBone.offset = Convert(bone->mOffsetMatrix);
                data.skeleton.bones.push_back(newBone);
            }

            // Update Vertices
            for (std::uint32_t j = 0; j != bone->mNumWeights; ++j) {
                auto& vertex = data.vertices[vertexCount + bone->mWeights[j].mVertexId];
                float weight = bone->mWeights[j].mWeight;
                AddBoneData(vertex, boneIndex, weight);
            }
        }

        vertexCount += mesh->mNumVertices;
    }

    // There may have been vertices that are acted on by more than 4 bones,
    // in which case the weights will not sum to one. Loop through and normalise.
    for (auto& vertex : data.vertices) {
        auto& v = vertex.boneWeights;
        v /= v.x + v.y + v.z + v.w;
    }

    // Initialise animation structures
    for (std::uint32_t i = 0; i != scene->mNumAnimations; ++i) {
        aiAnimation* animData = scene->mAnimations[i];
        std::string name(animData->mName.data);

        Animation& animation = data.skeleton.animations[name];
        animation.name = name;
        animation.keyFrames.resize(data.skeleton.bones.size());
    }

    // Load the skeleton, which consists of parent/child bone relations as
    // well as animations.
    LoadSkeleton(data.skeleton, scene, nullptr, scene->mRootNode, glm::mat4(1.0));

    return data;
}

static_mesh::static_mesh(const static_mesh_data& data)
    : d_vertex_buffer(0)
    , d_index_buffer(0)
    , d_vertex_count(data.indices.size())
{
    glCreateBuffers(1, &d_vertex_buffer);
    glNamedBufferData(d_vertex_buffer, sizeof(static_vertex) * data.vertices.size(), data.vertices.data(), GL_STATIC_DRAW);

    glCreateBuffers(1, &d_index_buffer);
    glNamedBufferData(d_index_buffer, sizeof(std::uint32_t) * data.indices.size(), data.indices.data(), GL_STATIC_DRAW);
}

static_mesh::~static_mesh()
{
    glDeleteBuffers(1, &d_vertex_buffer);
    glDeleteBuffers(1, &d_index_buffer);
}

std::unique_ptr<static_mesh> static_mesh::from_data(const static_mesh_data& data)
{
    return std::make_unique<static_mesh>(data);
}

std::unique_ptr<static_mesh> static_mesh::from_file(const std::string& file)
{
    return static_mesh::from_data(static_mesh_data::load(file));
}

void static_mesh::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, d_vertex_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_index_buffer);

    for (int index : std::views::iota(0, 5)) {
        glEnableVertexAttribArray(index);
        glVertexAttribDivisor(index, 0);
    } 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(static_vertex), (void*)offsetof(static_vertex, position));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(static_vertex), (void*)offsetof(static_vertex, textureCoords));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(static_vertex), (void*)offsetof(static_vertex, normal));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(static_vertex), (void*)offsetof(static_vertex, tangent));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(static_vertex), (void*)offsetof(static_vertex, bitangent));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


animated_mesh::animated_mesh(const animated_mesh_data& data)
    : d_vertex_buffer(0)
    , d_index_buffer(0)
    , d_vertex_count(data.indices.size())
    , d_skeleton(data.skeleton)
{
    glCreateBuffers(1, &d_vertex_buffer);
    glNamedBufferData(d_vertex_buffer, sizeof(animated_vertex) * data.vertices.size(), data.vertices.data(), GL_STATIC_DRAW);

    glCreateBuffers(1, &d_index_buffer);
    glNamedBufferData(d_index_buffer, sizeof(std::uint32_t) * data.indices.size(), data.indices.data(), GL_STATIC_DRAW);
}

animated_mesh::~animated_mesh()
{
    glDeleteBuffers(1, &d_vertex_buffer);
    glDeleteBuffers(1, &d_index_buffer);
}

std::unique_ptr<animated_mesh> animated_mesh::from_data(const animated_mesh_data& data)
{
    return std::make_unique<animated_mesh>(data);
}

std::unique_ptr<animated_mesh> animated_mesh::from_file(const std::string& file)
{
    return animated_mesh::from_data(animated_mesh_data::load(file));
}

void animated_mesh::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, d_vertex_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_index_buffer);
    
    for (int index : std::views::iota(0, 7)) {
        glEnableVertexAttribArray(index);
        glVertexAttribDivisor(index, 0);
    }

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(animated_vertex), (void*)offsetof(animated_vertex, position));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(animated_vertex), (void*)offsetof(animated_vertex, textureCoords));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(animated_vertex), (void*)offsetof(animated_vertex, normal));
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(animated_vertex), (void*)offsetof(animated_vertex, tangent));
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(animated_vertex), (void*)offsetof(animated_vertex, bitangent));
    glVertexAttribIPointer(5, 4, GL_INT, sizeof(animated_vertex), (void*)offsetof(animated_vertex, boneIndices));
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(animated_vertex), (void*)offsetof(animated_vertex, boneWeights));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

std::vector<glm::mat4> animated_mesh::get_pose(const std::string& name, float time) const
{
    return d_skeleton.GetPose(name, time);
}

std::vector<std::string> animated_mesh::get_animation_names() const
{
    std::vector<std::string> names;
    names.reserve(d_skeleton.animations.size());
    for (const auto& [name, animation] : d_skeleton.animations) {
        names.push_back(name);
    }
    return names;
}

}