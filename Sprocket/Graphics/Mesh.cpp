#include "Mesh.h"

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

void AddBoneData(AnimVertex& vertex, std::uint32_t index, float weight)
{
    for (std::uint32_t i = 0; i != 4; ++i) {
        if (vertex.boneIndices[i] == -1) {
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
}

Animation GetAnimation(Skeleton& skeleton, aiAnimation* animationData)
{
    Animation animation;
    animation.name = std::string(animationData->mName.data);
    animation.duration = animationData->mDuration;
    animation.boneKeyFramesPos.resize(skeleton.bones.size());
    animation.boneKeyFramesOri.resize(skeleton.bones.size());

    for (std::uint32_t index = 0; index != skeleton.bones.size(); ++index) {
        const Bone& bone = skeleton.bones[index];
        aiNodeAnim* nodeAnim = GetNodeAnim(animationData, bone.name);

        for (std::uint32_t i = 0; i != nodeAnim->mNumPositionKeys; ++i) {
            auto& pos = nodeAnim->mPositionKeys[i];
            animation.boneKeyFramesPos[index].push_back({
                (float)pos.mTime, Convert(pos.mValue)
            });
        }

        for (std::uint32_t i = 0; i != nodeAnim->mNumRotationKeys; ++i) {
            auto& pos = nodeAnim->mRotationKeys[i];
            animation.boneKeyFramesOri[index].push_back({
                (float)pos.mTime, Convert(pos.mValue)
            });
        }
    }

    return animation;
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

// The recursive function for loading skeleton and animation data.
void LoadSkeleton(
    Skeleton& skeleton,
    const aiScene* scene,
    const aiNode* lastBoneNode, // The last node that contained a bone, starts out null
    const aiNode* currentNode,  // The current node that we are dealing with.
    const Maths::mat4& parentTransform
)
{
    assert(currentNode);
    bool isBone = IsBone(skeleton, currentNode);

    const aiNode* currentBoneNode = lastBoneNode;
    Maths::mat4 nodeTransform = parentTransform * Convert(currentNode->mTransformation);
    if (isBone) {
        currentBoneNode = currentNode;
        nodeTransform = Maths::mat4(1.0);
    }

    Bone* currentBone = GetBone(skeleton, currentBoneNode);
    for (std::uint32_t i = 0; i != currentNode->mNumChildren; ++i) {
        aiNode* childNode = currentNode->mChildren[i];
        Bone* childBone = GetBone(skeleton, childNode);

        if (childBone && currentBone) {
            currentBone->children.push_back(childBone->index);
        }

        LoadSkeleton(skeleton, scene, currentBoneNode, childNode, nodeTransform);
    }
}

std::shared_ptr<Mesh> LoadStaticMesh(const aiScene* scene)
{    
    StaticMeshData data;

    for (std::size_t idx = 0; idx != scene->mNumMeshes; ++idx) {
        aiMesh* mesh = scene->mMeshes[idx];

        // Vertices
        for (unsigned int i = 0; i != mesh->mNumVertices; ++i) {
            Vertex vertex;
            vertex.position = Convert(mesh->mVertices[i]);
            vertex.normal = Convert(mesh->mNormals[i]);
            vertex.textureCoords = Convert(mesh->mTextureCoords[0][i]);
            
            if (mesh->HasTangentsAndBitangents()) {
                vertex.tangent = Convert(mesh->mTangents[i]);
                vertex.bitangent = Convert(mesh->mBitangents[i]);
            }

            data.vertices.push_back(vertex);
        }

        // Indices
        for (unsigned int i = 0; i != mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j != face.mNumIndices; ++j) {
                data.indices.push_back(face.mIndices[j]);
            }
        }
    }
    return std::make_shared<Mesh>(data);
}

std::shared_ptr<Mesh> LoadAnimatedMesh(std::shared_ptr<Assimp::Importer> importer, const aiScene* scene)
{    
    AnimatedMeshData data;
    data.importer = importer;
    auto& skel = data.skeleton;
    SPKT_LOG_INFO("Loading animated mesh");

    std::uint32_t vertexCount = 0;

    for (std::size_t idx = 0; idx != scene->mNumMeshes; ++idx) {
        aiMesh* mesh = scene->mMeshes[idx];

        // Vertices
        for (std::uint32_t i = 0; i != mesh->mNumVertices; ++i) {
            AnimVertex vertex;
            vertex.position = Convert(mesh->mVertices[i]);
            vertex.normal = Convert(mesh->mNormals[i]);
            vertex.textureCoords = Convert(mesh->mTextureCoords[0][i]);
            
            if (mesh->HasTangentsAndBitangents()) {
                vertex.tangent = Convert(mesh->mTangents[i]);
                vertex.bitangent = Convert(mesh->mBitangents[i]);
            }

            data.vertices.push_back(vertex);
        }

        // Indices
        for (std::uint32_t i = 0; i != mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (std::uint32_t j = 0; j != face.mNumIndices; ++j) {
                data.indices.push_back(face.mIndices[j]);
            }
        }

        // Bones
        for (std::uint32_t i = 0; i != mesh->mNumBones; ++i) {
            aiBone* bone = mesh->mBones[i];
            std::string boneName(bone->mName.data);

            // We have to do this lookup as a bone be shared by multiple
            // submeshes, so we may have already encountered this bone.
            int boneIndex = 0;
            if (auto it = skel.boneMap.find(boneName); it != skel.boneMap.end()) {
                boneIndex = it->second;
            } else {
                boneIndex = data.skeleton.bones.size();
                Bone newBone;
                newBone.name = boneName;
                newBone.index = boneIndex;
                newBone.transform = Convert(bone->mOffsetMatrix);
                skel.bones.push_back(newBone);
                skel.boneMap[boneName] = boneIndex;
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

    LoadSkeleton(skel, scene, nullptr, scene->mRootNode, Maths::mat4(1.0));
    for (std::uint32_t i = 0; i != skel.bones.size(); ++i) {
        Bone& bone = skel.bones[i];
        assert(i == bone.index);
        SPKT_LOG_INFO("Bone {}, index {}", bone.name, bone.index);
        SPKT_LOG_INFO("Children:");
        for (const auto& child : bone.children) {
            SPKT_LOG_INFO("  - {}", child);
        }
    }

    //for (std::uint32_t i = 0; i != scene->mNumAnimations; ++i) {
    //    aiAnimation* animationData = scene->mAnimations[i];
    //    std::string name(animationData->mName.data);
    //    data.animations[name] = GetAnimation(skel, animationData);
    //}

    return std::make_shared<Mesh>(data);
}

}

Mesh::Mesh(const StaticMeshData& data)
    : d_vertexBuffer(0)
    , d_indexBuffer(0)
    , d_vertexCount(data.indices.size())
    , d_layout(sizeof(Vertex), 0)
    , d_animated(false)
{
    glCreateBuffers(1, &d_vertexBuffer);
    glNamedBufferData(d_vertexBuffer, sizeof(Vertex) * data.vertices.size(), data.vertices.data(), GL_STATIC_DRAW);

    glCreateBuffers(1, &d_indexBuffer);
    glNamedBufferData(d_indexBuffer, sizeof(std::uint32_t) * data.indices.size(), data.indices.data(), GL_STATIC_DRAW);

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
    , d_animated(true)
    , d_skeleton(data.skeleton)
    , d_animations(data.animations)
    , d_importer(data.importer)
{
    glCreateBuffers(1, &d_vertexBuffer);
    glNamedBufferData(d_vertexBuffer, sizeof(AnimVertex) * data.vertices.size(), data.vertices.data(), GL_STATIC_DRAW);

    glCreateBuffers(1, &d_indexBuffer);
    glNamedBufferData(d_indexBuffer, sizeof(std::uint32_t) * data.indices.size(), data.indices.data(), GL_STATIC_DRAW);

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
    , d_animated(false)
{
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

std::shared_ptr<Mesh> Mesh::FromFile(const std::string& file)
{
    auto importer = std::make_shared<Assimp::Importer>();
    const aiScene* scene = importer->ReadFile(file, GetAssimpFlags());

    if (!IsSceneValid(scene)) {
        SPKT_LOG_ERROR("ERROR::ASSIMP::{}", importer->GetErrorString());
        return std::make_shared<Mesh>();
    }

    if (scene->HasAnimations()) {
        return LoadAnimatedMesh(importer, scene);
    }
    return LoadStaticMesh(scene);
}

void Mesh::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, d_vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, d_indexBuffer);
    d_layout.SetAttributes();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool Mesh::operator==(const Mesh& other) const
{
    // Two models are the same if they point to the same VAO.
    return d_vertexBuffer == other.d_vertexBuffer &&
           d_indexBuffer == other.d_indexBuffer;
}

BufferLayout Mesh::GetLayout() const
{
    return d_layout;
}

void Mesh::SetAnimation(const std::string& name)
{
    d_activeAnimation = name;
}

void Mesh::SetPose(float time)
{
    UpdateTransforms(time, d_importer->GetScene()->mRootNode, Maths::mat4(1.0));
}

std::vector<Maths::mat4> Mesh::GetBoneTransforms() const
{
    std::vector<Maths::mat4> transforms;
    transforms.reserve(d_skeleton.bones.size());
    for (const auto& bone : d_skeleton.bones) {
        transforms.push_back(bone.finalTransform);
    }
    return transforms;
}

std::vector<std::string> Mesh::GetAnimationNames() const
{
    std::vector<std::string> names;
    names.reserve(d_animations.size());
    for (const auto& [name, animation] : d_animations) {
        names.push_back(name);
    }
    return names;
}

void Mesh::UpdateTransforms(float time, aiNode* node, const Maths::mat4& parentTransform)
{
    std::string name(node->mName.data);
    const aiAnimation* animation = d_importer->GetScene()->mAnimations[0];
 
    // If the name is the name of a bone, then this node in the tree represents
    // a bone and there will be an aiNodeAnim struct associated containing the
    // keyframe data. If it does not represent a bone, the node just contains
    // a transform that applies to all child nodes (and hence bones).
    aiNodeAnim* nodeAnim = nullptr;
    for (std::uint32_t i = 0; i != animation->mNumChannels; ++i) {
        aiNodeAnim* x = animation->mChannels[i];
        if (std::string(x->mNodeName.data) == name) {
            nodeAnim = x;
            break;
        }
    }

    Maths::mat4 nodeTransform(1.0);
    if (nodeAnim) { // Current node represents a bone
        nodeTransform; // TODO: Interpolate between keyframes
    } else { // No bone, just a transform to apply to sub-bones.
        nodeTransform = Convert(node->mTransformation);
    }

    Maths::mat4 transform = parentTransform * nodeTransform;
}

}