#include "ModelManager.h"
#include "Log.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Sprocket {
namespace {

bool IsSceneValid(const aiScene* scene)
    // Returns true if the scene is valid and false otherwise.
{
    return scene && 
           !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) &&
           scene->mRootNode;
}

Model3D ProcessMesh(const aiScene* scene, aiMesh* mesh)
{    
    Vertex3DBuffer vertices;
    IndexBuffer    indices;

    // Vertices
    for (unsigned int i = 0; i != mesh->mNumVertices; ++i) {
        Vertex3D vertex;

        vertex.position.x = mesh->mVertices[i].x;
        vertex.position.y = mesh->mVertices[i].y;
        vertex.position.z = mesh->mVertices[i].z;

        vertex.normal.x = mesh->mNormals[i].x;
        vertex.normal.y = mesh->mNormals[i].y;
        vertex.normal.z = mesh->mNormals[i].z;

        // TODO: Add error checking code here to make sure the texture exists
        vertex.textureCoords.x = mesh->mTextureCoords[0][i].x;
        vertex.textureCoords.y = mesh->mTextureCoords[0][i].y;

        vertices.push_back(vertex);
    }

    // Indices
    for (unsigned int i = 0; i != mesh->mNumFaces; ++i) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j != face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return Model3D(vertices, indices);
}

}

Model3D ModelManager::LoadModel(const std::string& path)
{
    Assimp::Importer importer;
    int flags = aiProcess_Triangulate | aiProcess_FlipUVs;
    const aiScene* scene = importer.ReadFile(path, flags);

    if (!IsSceneValid(scene)) {
        SPKT_LOG_ERROR("ERROR::ASSIMP::{}", importer.GetErrorString());
        return Model3D();
    }

    if (scene->mNumMeshes != 1) {
        SPKT_LOG_ERROR("File does not have exactly one mesh, not supported!");
        return Model3D();
    }

    return ProcessMesh(scene, scene->mMeshes[0]);
}

Model3D ModelManager::GetModel(const std::string& path)
{
    if (path == "") { return Model3D(); }
    
    auto it = d_loadedModels.find(path);
    if (it != d_loadedModels.end()) {
        return it->second;
    }
    Model3D model = LoadModel(path);
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