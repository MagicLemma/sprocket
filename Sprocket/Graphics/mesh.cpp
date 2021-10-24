#include "mesh.h"

#include <Sprocket/Utility/Maths.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

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

bool is_scene_valid(const aiScene* scene)
    // Returns true if the scene is valid and false otherwise.
{
    return scene && 
           !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) &&
           scene->mRootNode &&
           scene->mNumMeshes > 0;
}

int get_assimp_flags()
{
    return aiProcess_Triangulate
         | aiProcess_FlipUVs
         | aiProcess_CalcTangentSpace
         | aiProcess_GenUVCoords
         | aiProcess_GenNormals
         | aiProcess_ValidateDataStructure;
}

void add_bone_data(animated_vertex& vertex, std::uint32_t index, float weight)
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

aiNodeAnim* get_node_anim(aiAnimation* animation, const std::string& name)
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

std::string node_name(const aiNode* node)
{
    if (!node) { return ""; }
    return std::string(node->mName.data);
}

bone* get_bone(skeleton& skeleton, const aiNode* node)
{
    if (!node) { return nullptr; }
    auto it = skeleton.bone_map.find(node_name(node));
    if (it != skeleton.bone_map.end()) {
        return &skeleton.bones[it->second];
    }
    return nullptr;
}

bool is_bone(const skeleton& skeleton, const aiNode* node)
{
    if (!node) { return false; }
    const auto it = skeleton.bone_map.find(node_name(node));
    return it != skeleton.bone_map.end();
}

void load_animations(skeleton& skeleton, bone* bone, const aiScene* scene, const glm::mat4& transform)
{
    assert(bone);
    for (std::uint32_t i = 0; i != scene->mNumAnimations; ++i) {
        aiAnimation* animation_data = scene->mAnimations[i];
        float ticks_per_second = animation_data->mTicksPerSecond;
        if (ticks_per_second == 0.0f) { ticks_per_second = 25.0f; } // If unknown

        const aiNodeAnim* key_frames = get_node_anim(animation_data, bone->name);
        std::string name(animation_data->mName.data);

        spkt::animation& animation = skeleton.animations[name];
        animation.duration = animation_data->mDuration / ticks_per_second;
        bone_key_frames& key_frame_data = animation.key_frames[bone->index];
        for (std::uint32_t i = 0; i != key_frames->mNumPositionKeys; ++i) {
            auto& x = key_frames->mPositionKeys[i];
            key_frame_data.positions.push_back({
                (float)x.mTime / ticks_per_second,
                ApplyTransform(transform, Convert(x.mValue))
            });
        }

        for (std::uint32_t i = 0; i != key_frames->mNumRotationKeys; ++i) {
            const auto& x = key_frames->mRotationKeys[i];
            key_frame_data.orientations.push_back({
                (float)x.mTime / ticks_per_second,
                glm::quat_cast(transform) * Convert(x.mValue)
            });
        }

        for (std::uint32_t i = 0; i != key_frames->mNumScalingKeys; ++i) {
            const auto& x = key_frames->mScalingKeys[i];
            key_frame_data.scales.push_back({
                (float)x.mTime/ticks_per_second,
                Convert(x.mValue)
            });
        }
    }

}

// The recursive function for loading skeleton and animation data.
void load_skeleton(
    skeleton& skeleton,
    const aiScene* scene,
    const aiNode* last_bone_node, // The last node that contained a bone, starts out null
    const aiNode* current_node,  // The current node that we are dealing with.
    const glm::mat4& parent_transform
)
// NOTE: The parentTransform is an artifact of Assimp's node tree. There may be nodes that don't
// correspond to bones but their transform matrices DO affect bones that are child nodes of it.
// In this algorithm, we keep track of this transform, and when we reach a bone, we apply the
// transform to all of the key frame data, and then pass the identity to its children. In
// essence, we "bake" the intermediate transforms into the animation data so that we do not need
// non-bone nodes in our skeleton tree.
{
    assert(current_node);

    glm::mat4 node_transform = parent_transform * Convert(current_node->mTransformation);
    if (is_bone(skeleton, current_node)) {
        // Reset the nodeTransform; the previous will get baked into this bones' animations.
        node_transform = glm::mat4(1.0);
        last_bone_node = current_node;

        spkt::bone* bone = get_bone(skeleton, last_bone_node);
        load_animations(skeleton, bone, scene, NoScale(parent_transform));
    }

    spkt::bone* current_bone = get_bone(skeleton, last_bone_node);
    for (std::uint32_t i = 0; i != current_node->mNumChildren; ++i) {
        aiNode*     child_node = current_node->mChildren[i];
        spkt::bone* child_bone = get_bone(skeleton, child_node);

        if (child_bone && current_bone) {
            current_bone->children.push_back(child_bone->index);
        }

        load_skeleton(skeleton, scene, last_bone_node, child_node, node_transform);
    }
}

}

static_mesh::static_mesh(const static_mesh_data& data)
    : d_vertices(data.vertices)
    , d_indices(data.indices)
{
}

static_mesh_data static_mesh::load(const std::string& file)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file, get_assimp_flags());
    assert(is_scene_valid(scene));
    static_mesh_data data;

    for (std::uint32_t idx = 0; idx != scene->mNumMeshes; ++idx) {
        const aiMesh* mesh = scene->mMeshes[idx];

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

void static_mesh::bind() const
{
    d_vertices.bind();
    d_indices.bind();
}


animated_mesh::animated_mesh(const animated_mesh_data& data)
    : d_vertices(data.vertices)
    , d_indices(data.indices)
    , d_skeleton(data.skeleton)
{
}

animated_mesh_data animated_mesh::load(const std::string& file)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file, get_assimp_flags());
    assert(is_scene_valid(scene));
    animated_mesh_data data;

    std::uint32_t vertex_count = 0;
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
                data.indices.push_back(vertex_count + face.mIndices[j]);
            }
        }

        // Bones
        for (std::uint32_t i = 0; i != mesh->mNumBones; ++i) {
            aiBone* ai_bone = mesh->mBones[i];
            std::string bone_name{ai_bone->mName.data};

            // We have to do this lookup as a bone be shared by multiple
            // submeshes, so we may have already encountered this bone.
            std::uint32_t bone_index = 0;
            auto it = data.skeleton.bone_map.find(bone_name);
            if (it != data.skeleton.bone_map.end()) {
                bone_index = data.skeleton.bone_map[bone_name];
            } else {
                bone_index = data.skeleton.bones.size();
                data.skeleton.bone_map[bone_name] = bone_index;
                
                spkt::bone newBone;
                newBone.name = bone_name;
                newBone.index = bone_index;
                newBone.offset = Convert(ai_bone->mOffsetMatrix);
                data.skeleton.bones.push_back(newBone);
            }

            // Update Vertices
            for (std::uint32_t j = 0; j != ai_bone->mNumWeights; ++j) {
                auto& vertex = data.vertices[vertex_count + ai_bone->mWeights[j].mVertexId];
                float weight = ai_bone->mWeights[j].mWeight;
                add_bone_data(vertex, bone_index, weight);
            }
        }

        vertex_count += mesh->mNumVertices;
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

        spkt::animation& animation = data.skeleton.animations[name];
        animation.name = name;
        animation.key_frames.resize(data.skeleton.bones.size());
    }

    // Load the skeleton, which consists of parent/child bone relations as
    // well as animations.
    load_skeleton(data.skeleton, scene, nullptr, scene->mRootNode, glm::mat4(1.0));

    return data;
}

void animated_mesh::bind() const
{
    d_vertices.bind();
    d_indices.bind();
}

std::vector<glm::mat4> animated_mesh::get_pose(const std::string& name, float time) const
{
    return d_skeleton.get_pose(name, time);
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