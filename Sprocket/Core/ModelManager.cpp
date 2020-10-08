#include "ModelManager.h"
#include "Log.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Sprocket {
namespace {

Maths::vec2 Convert(const aiVector2D& v)
{
    return Maths::vec2{v.x, v.y};
}

Maths::vec3 Convert(const aiVector3D& v)
{
    return Maths::vec3{v.x, v.y, v.z};
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

std::shared_ptr<Mesh> LoadStaticMesh(const aiScene* scene)
{    
    VertexBuffer vertices;
    IndexBuffer    indices;

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

            vertices.push_back(vertex);
        }

        // Indices
        for (unsigned int i = 0; i != mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j != face.mNumIndices; ++j) {
                indices.push_back(face.mIndices[j]);
            }
        }

    }

    return std::make_shared<Mesh>(vertices, indices);
}

}

ModelManager::ModelManager()
    : d_default(std::make_shared<Mesh>())
{}

std::shared_ptr<Mesh> ModelManager::LoadModel(const std::string& path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, GetAssimpFlags());

    if (!IsSceneValid(scene)) {
        SPKT_LOG_ERROR("ERROR::ASSIMP::{}", importer.GetErrorString());
        return std::make_shared<Mesh>();
    }

    return LoadStaticMesh(scene);
}

std::shared_ptr<Mesh> ModelManager::GetModel(const std::string& path)
{
    if (path == "") { return d_default; }
    
    auto it = d_loadedModels.find(path);
    if (it != d_loadedModels.end()) {
        return it->second;
    }
    auto model = LoadModel(path);
    d_loadedModels.emplace(path, model);
    return model;   
}

ModelManager::Map::iterator ModelManager::begin()
{
    return d_loadedModels.begin();
}

ModelManager::Map::iterator ModelManager::end()
{
    return d_loadedModels.end();
}

ModelManager::Map::const_iterator ModelManager::cbegin() const
{
    return d_loadedModels.cbegin();
}

ModelManager::Map::const_iterator ModelManager::cend() const
{
    return d_loadedModels.cend();
}


}