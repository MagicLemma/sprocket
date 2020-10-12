#include "Mesh.h"

#include <cassert>
#include <glad/glad.h>

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

Mesh::Mesh(const VertexBuffer& vertices, const IndexBuffer& indices)
    : d_vertexBuffer(0)
    , d_indexBuffer(0)
    , d_vertexCount(indices.size())
    , d_layout(sizeof(Vertex), 0)
{
    glCreateBuffers(1, &d_vertexBuffer);
    glNamedBufferData(d_vertexBuffer, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glCreateBuffers(1, &d_indexBuffer);
    glNamedBufferData(d_indexBuffer, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 2);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    d_layout.AddAttribute(DataType::FLOAT, 3);
    assert(d_layout.Validate());
}

Mesh::Mesh()
    : d_vertexBuffer(0)
    , d_indexBuffer(0)
    , d_vertexCount(0)
    , d_layout(sizeof(Vertex), 0)
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
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(file, GetAssimpFlags());

    if (!IsSceneValid(scene)) {
        SPKT_LOG_ERROR("ERROR::ASSIMP::{}", importer.GetErrorString());
        return std::make_shared<Mesh>();
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

}