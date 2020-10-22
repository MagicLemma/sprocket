#include "Mesh.h"
#include "Types.h"
#include "Maths.h"

#include <cassert>
#include <glad/glad.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Sprocket {
namespace {

Maths::mat4 Convert(const aiMatrix4x4& matrix)
{
    Maths::mat4 result;

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

Maths::vec2 Convert(const aiVector2D& v)
{
    return Maths::vec2{v.x, v.y};
}

Maths::vec3 Convert(const aiVector3D& v)
{
    return Maths::vec3{v.x, v.y, v.z};
}

Maths::quat Convert(const aiQuaternion& q)
{
    Maths::quat quat;
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

void AddBoneData(AnimVertex& vertex, u32 index, f32 weight)
{
    for (u32 i = 0; i != 4; ++i) {
        if (vertex.boneIndices[i] == -1) {
            vertex.boneIndices[i] = index;
            vertex.boneWeights[i] = weight;
            return;
        }
    }
    
    // Already got four acting bones, so check if any are less influential
    // than this one and replace it if so.
    for (u32 i = 0; i != 4; ++i) {
        if (vertex.boneWeights[i] < weight) {
            vertex.boneIndices[i] = index;
            vertex.boneWeights[i] = weight;
            return;
        }
    }
}

aiNodeAnim* GetNodeAnim(aiAnimation* animation, const std::string& name)
{
    for (u32 i = 0; i != animation->mNumChannels; ++i) {
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

void LoadAnimations(
    Skeleton& skeleton,
    Bone* bone,
    const aiScene* scene,
    const Maths::mat4& transform
)
{
    assert(bone);
    for (u32 i = 0; i != scene->mNumAnimations; ++i) {
        aiAnimation* animationData = scene->mAnimations[i];
        f32 ticksPerSec = animationData->mTicksPerSecond;
        if (ticksPerSec == 0.0f) { ticksPerSec = 25.0f; } // If unknown

        const aiNodeAnim* keyFrames = GetNodeAnim(animationData, bone->name);
        std::string name(animationData->mName.data);

        Animation& animation = skeleton.animations[name];
        animation.duration = animationData->mDuration / ticksPerSec;
        BoneKeyFrames& keyFrameData = animation.keyFrames[bone->index];
        for (u32 i = 0; i != keyFrames->mNumPositionKeys; ++i) {
            auto& x = keyFrames->mPositionKeys[i];
            keyFrameData.keyPostitions.push_back({
                (float)x.mTime / ticksPerSec,
                Maths::ApplyTransform(transform, Convert(x.mValue))
            });
        }

        for (u32 i = 0; i != keyFrames->mNumRotationKeys; ++i) {
            auto& x = keyFrames->mRotationKeys[i];
            keyFrameData.keyOrientations.push_back({
                (float)x.mTime / ticksPerSec,
                Maths::ToQuat(transform) * Convert(x.mValue)
            });
        }

        for (u32 i = 0; i != keyFrames->mNumScalingKeys; ++i) {
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
    const Maths::mat4& parentTransform
)
// NOTE: The parentTransform is an artifact of Assimp's node tree. There may be nodes that don't
// correspond to bones but their transform matrices DO affect bones that are child nodes of it.
// In this algorithm, we keep track of this transform, and when we reach a bone, we apply the
// transform to all of the key frame data, and then pass the identity to its children. In
// essence, we "bake" the intermediate transforms into the animation data so that we do not need
// non-bone nodes in our skeleton tree.
{
    assert(currentNode);

    Maths::mat4 nodeTransform = parentTransform * Convert(currentNode->mTransformation);
    if (IsBone(skeleton, currentNode)) {
        // Reset the nodeTransform; the previous will get baked into this bones' animations.
        nodeTransform = Maths::mat4(1.0);
        lastBoneNode = currentNode;

        Bone* bone = GetBone(skeleton, lastBoneNode);
        LoadAnimations(skeleton, bone, scene, Maths::NoScale(parentTransform));
    }

    Bone* currentBone = GetBone(skeleton, lastBoneNode);
    for (u32 i = 0; i != currentNode->mNumChildren; ++i) {
        aiNode* childNode = currentNode->mChildren[i];

        Bone* childBone = GetBone(skeleton, childNode);
        if (childBone && currentBone) {
            currentBone->children.push_back(childBone->index);
        }

        LoadSkeleton(skeleton, scene, lastBoneNode, childNode, nodeTransform);
    }
}

StaticMeshData LoadStaticMesh(const aiScene* scene)
{    
    StaticMeshData data;

    for (u32 idx = 0; idx != scene->mNumMeshes; ++idx) {
        aiMesh* mesh = scene->mMeshes[idx];

        // Vertices
        for (u32 i = 0; i != mesh->mNumVertices; ++i) {
            Vertex vertex;
            vertex.position = Convert(mesh->mVertices[i]);
            vertex.normal = Convert(mesh->mNormals[i]);
            vertex.textureCoords = Convert(mesh->mTextureCoords[0][i]);
            vertex.tangent = Convert(mesh->mTangents[i]);
            vertex.bitangent = Convert(mesh->mBitangents[i]);
            data.vertices.push_back(vertex);
        }

        // Indices
        for (u32 i = 0; i != mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (u32 j = 0; j != face.mNumIndices; ++j) {
                data.indices.push_back(face.mIndices[j]);
            }
        }
    }

    return data;
}

AnimatedMeshData LoadAnimatedMesh(const aiScene* scene)
{    
    AnimatedMeshData data;

    u32 vertexCount = 0;
    for (u32 idx = 0; idx != scene->mNumMeshes; ++idx) {
        aiMesh* mesh = scene->mMeshes[idx];

        // Vertices
        for (u32 i = 0; i != mesh->mNumVertices; ++i) {
            AnimVertex vertex;
            vertex.position = Convert(mesh->mVertices[i]);
            vertex.normal = Convert(mesh->mNormals[i]);
            vertex.textureCoords = Convert(mesh->mTextureCoords[0][i]);
            vertex.tangent = Convert(mesh->mTangents[i]);
            vertex.bitangent = Convert(mesh->mBitangents[i]);
            data.vertices.push_back(vertex);
        }

        // Indices
        for (u32 i = 0; i != mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (u32 j = 0; j != face.mNumIndices; ++j) {
                data.indices.push_back(vertexCount + face.mIndices[j]);
            }
        }

        // Bones
        for (u32 i = 0; i != mesh->mNumBones; ++i) {
            aiBone* bone = mesh->mBones[i];
            std::string boneName(bone->mName.data);

            // We have to do this lookup as a bone be shared by multiple
            // submeshes, so we may have already encountered this bone.
            u32 boneIndex = 0;
            if (data.skeleton.boneMap.contains(boneName)) {
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
            for (u32 j = 0; j != bone->mNumWeights; ++j) {
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
    for (u32 i = 0; i != scene->mNumAnimations; ++i) {
        aiAnimation* animData = scene->mAnimations[i];
        std::string name(animData->mName.data);

        Animation& animation = data.skeleton.animations[name];
        animation.name = name;
        animation.keyFrames.resize(data.skeleton.bones.size());
    }

    // Load the skeleton, which consists of parent/child bone relations as
    // well as animations.
    LoadSkeleton(data.skeleton, scene, nullptr, scene->mRootNode, Maths::mat4(1.0));

    return data;
}

}

MeshData::MeshData(const std::string& file)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file, GetAssimpFlags());
    assert(IsSceneValid(scene));

    if (scene->HasAnimations()) {
        data = LoadAnimatedMesh(scene);
    }
    else {
        data = LoadStaticMesh(scene);
    }
}

Mesh::Mesh(const StaticMeshData& data)
    : d_vertexBuffer(0)
    , d_indexBuffer(0)
    , d_vertexCount(data.indices.size())
    , d_layout(sizeof(Vertex), 0)
{
    glCreateBuffers(1, &d_vertexBuffer);
    glNamedBufferData(d_vertexBuffer, sizeof(Vertex) * data.vertices.size(), data.vertices.data(), GL_STATIC_DRAW);

    glCreateBuffers(1, &d_indexBuffer);
    glNamedBufferData(d_indexBuffer, sizeof(u32) * data.indices.size(), data.indices.data(), GL_STATIC_DRAW);

    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 2);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    assert(d_layout.Validate());
}

Mesh::Mesh(const AnimatedMeshData& data)
    : d_vertexBuffer(0)
    , d_indexBuffer(0)
    , d_vertexCount(data.indices.size())
    , d_layout(sizeof(AnimVertex), 0)
    , d_skeleton(data.skeleton)
{
    glCreateBuffers(1, &d_vertexBuffer);
    glNamedBufferData(d_vertexBuffer, sizeof(AnimVertex) * data.vertices.size(), data.vertices.data(), GL_STATIC_DRAW);

    glCreateBuffers(1, &d_indexBuffer);
    glNamedBufferData(d_indexBuffer, sizeof(u32) * data.indices.size(), data.indices.data(), GL_STATIC_DRAW);

    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 2);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::INT,   4);
    d_layout.AddAttribute(DataType::FLOAT, 4);
    assert(d_layout.Validate());
}

Mesh::Mesh()
    : d_vertexBuffer(0)
    , d_indexBuffer(0)
    , d_vertexCount(0)
    , d_layout(sizeof(Vertex), 0)
{
    glCreateBuffers(1, &d_vertexBuffer);
    glNamedBufferData(d_vertexBuffer, 0, nullptr, GL_STATIC_DRAW);

    glCreateBuffers(1, &d_indexBuffer);
    glNamedBufferData(d_indexBuffer, 0, nullptr, GL_STATIC_DRAW);

    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 2);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    assert(d_layout.Validate());
}

Mesh::~Mesh()
{
    if (d_vertexBuffer != 0) {
        glDeleteBuffers(1, &d_vertexBuffer);
    }
    if (d_indexBuffer != 0) {
        glDeleteBuffers(1, &d_indexBuffer);
    }
}

std::shared_ptr<Mesh> Mesh::FromData(const MeshData& data)
{
    return std::visit([&](auto&& data) {
        return std::make_shared<Mesh>(data);
    }, data.data);
}

std::shared_ptr<Mesh> Mesh::FromFile(const std::string& file)
{
    return Mesh::FromData(MeshData(file));
}

void Mesh::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, d_vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_indexBuffer);
    d_layout.SetAttributes();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

BufferLayout Mesh::GetLayout() const
{
    return d_layout;
}

std::vector<Maths::mat4> Mesh::GetPose(const std::string& name, f32 time) const
{
    if (d_skeleton.has_value()) {
        return d_skeleton.value().GetPose(name, time);
    }
    return {};
}

std::vector<std::string> Mesh::GetAnimationNames() const
{
    std::vector<std::string> names;
    if (d_skeleton.has_value()) {
        names.reserve(d_skeleton.value().animations.size());
        for (const auto& [name, animation] : d_skeleton.value().animations) {
            names.push_back(name);
        }
    }
    return names;
}

}