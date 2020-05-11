#include "ModelLoader.h"
#include "Log.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Sprocket {
namespace {

bool isSceneValid(const aiScene* scene)
    // Returns true if the scene is valid and false otherwise.
{
    return scene && 
           !(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) &&
           scene->mRootNode;
}

ModelLoaderData processMesh(const aiScene* scene, aiMesh* mesh)
{
    ModelLoaderData mld;

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

    mld.model = Model3D(vertices, indices);
    return mld;
}

void processNode(std::vector<ModelLoaderData>* data,
                 const aiScene* scene,
                 aiNode* node)
{
    for (unsigned int i = 0; i != node->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        data->push_back(processMesh(scene, mesh));
    }

    for (unsigned int i = 0; i != node->mNumChildren; ++i) {
        processNode(data, scene, node->mChildren[i]);
    }
}

}

std::vector<ModelLoaderData> ModelLoader::loadModel(const std::string& path)
{
    std::vector<ModelLoaderData> data;

    Assimp::Importer importer;
    int flags = aiProcess_Triangulate | aiProcess_FlipUVs;
    const aiScene* scene = importer.ReadFile(path, flags);

    if (!isSceneValid(scene)) {
        SPKT_LOG_ERROR("ERROR::ASSIMP::{}", importer.GetErrorString());
        return data;
    }

    processNode(&data, scene, scene->mRootNode);
    return data;
}

}