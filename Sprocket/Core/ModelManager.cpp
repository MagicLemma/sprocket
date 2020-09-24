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
           scene->mRootNode;
}

std::shared_ptr<Model3D> ProcessMesh(const aiScene* scene)
{    
    Vertex3DBuffer vertices;
    IndexBuffer    indices;

    for (std::size_t idx = 0; idx != scene->mNumMeshes; ++idx) {
        aiMesh* mesh = scene->mMeshes[idx];

        // Vertices
        for (unsigned int i = 0; i != mesh->mNumVertices; ++i) {
            Vertex3D vertex;
            vertex.position = Convert(mesh->mVertices[i]);
            vertex.normal = Convert(mesh->mNormals[i]);
            
            if (mesh->HasTangentsAndBitangents()) {
                vertex.tangent = Convert(mesh->mTangents[i]);
                vertex.bitangent = Convert(mesh->mBitangents[i]);
            }

            vertex.textureCoords = Convert(mesh->mTextureCoords[0][i]);
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

    return std::make_shared<Model3D>(vertices, indices);
}

}

std::shared_ptr<Model3D> ModelManager::LoadModel(const std::string& path)
{
    Assimp::Importer importer;
    int flags = 
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace |
        aiProcess_GenUVCoords |
        aiProcess_GenNormals |
        aiProcess_ValidateDataStructure;

    const aiScene* scene = importer.ReadFile(path, flags);

    if (!IsSceneValid(scene)) {
        SPKT_LOG_ERROR("ERROR::ASSIMP::{}", importer.GetErrorString());
        return std::make_shared<Model3D>();
    }

    if (scene->mNumMeshes < 1) {
        SPKT_LOG_ERROR("File has no mesh!");
        return std::make_shared<Model3D>();
    }

    return ProcessMesh(scene);
}

std::shared_ptr<Model3D> ModelManager::GetModel(const std::string& path)
{
    if (path == "") { return std::make_shared<Model3D>(); }
    
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