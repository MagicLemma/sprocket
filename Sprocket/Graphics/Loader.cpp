#include "Graphics/Loader.h"
#include "Graphics/Vertex.h"
#include "Graphics/ObjParser.h"
#include "Utility/Log.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <cstddef>
#include <unordered_map>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Sprocket {
namespace Loader {

namespace {

std::vector<unsigned int> s_vaoList;
std::vector<unsigned int> s_vboList;
std::vector<unsigned int> s_texList;

std::unordered_map<std::string, ModelPtr> s_models;
std::unordered_map<std::string, TexturePtr> s_textures;

unsigned int createVAO()
{
    unsigned int vaoId;
    glGenVertexArrays(1, &vaoId);
    s_vaoList.push_back(vaoId);
    glBindVertexArray(vaoId);
    return vaoId;
}

void unbindVAO()
{
    glBindVertexArray(0);
}

void bindVertexBuffer(const VertexBuffer& vertexBuffer)
{
    unsigned int vboId;
    glGenBuffers(1, &vboId);
    s_vboList.push_back(vboId);

    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexBuffer.size(), vertexBuffer.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(Vertex::posAttr, Vertex::posCount, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, position)));
                                           
    glVertexAttribPointer(Vertex::texAttr, Vertex::texCount, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, texture)));

    glVertexAttribPointer(Vertex::norAttr, Vertex::norCount, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, normal)));
}

void bindIndexBuffer(const IndexBuffer& indexBuffer)
{
    unsigned int vboId;
    glGenBuffers(1, &vboId);
    s_vboList.push_back(vboId);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexBuffer.size(), indexBuffer.data(),
                 GL_STATIC_DRAW);
}

void bindVertex2DBuffer(const Vertex2DBuffer& vertex2DBuffer)
{
    unsigned int vboId;
    glGenBuffers(1, &vboId);
    s_vboList.push_back(vboId);

    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * vertex2DBuffer.size(), vertex2DBuffer.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(Vertex2D::posAttr, Vertex2D::posCount, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),
                          reinterpret_cast<void*>(offsetof(Vertex2D, position)));
}

}

void init()
{
    SPKT_LOG_INFO("Initialised Loader");
}

void deinit()
{
    SPKT_LOG_INFO("Deinitialised Loader: Cleaning up VAOs, VBOs and textures");
    glDeleteVertexArrays(s_vaoList.size(), s_vaoList.data());
    glDeleteBuffers(s_vboList.size(), s_vboList.data());
    glDeleteTextures(s_texList.size(), s_texList.data());
}

ModelPtr loadModel(const std::string& objFile)
{
    auto [vertices, indices] = parseObjFile(objFile);

    unsigned int vaoId = createVAO();
    bindVertexBuffer(vertices);
    bindIndexBuffer(indices);
    unbindVAO();

    return std::make_shared<Model>(vaoId, indices.size(), ModelType::ENTITY);
}

TexturePtr loadTexture(const std::string& textureFile)
{
    unsigned int texId;
    glGenTextures(1, &texId);
    s_texList.push_back(texId);

    stbi_set_flip_vertically_on_load(1);
    int width, height, bpp;
    unsigned char* data = stbi_load(textureFile.c_str(), &width, &height, &bpp, 4);

    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D,
                 0, GL_RGBA8, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return std::make_shared<Texture>(texId);
}

ModelPtr load2DModel(const Vertex2DBuffer& vertex2DBuffer)
{
    unsigned int vaoId = createVAO();
    bindVertex2DBuffer(vertex2DBuffer);
    unbindVAO();

    return std::make_shared<Model>(vaoId, vertex2DBuffer.size(), ModelType::FLAT);
}

ModelPtr loadModel(const std::string& name, const std::string& objFile)
{
    auto model = loadModel(objFile);
    s_models[name] = model;
    return model;
}

TexturePtr loadTexture(const std::string& name, const std::string& textureFile)
{
    auto texture = loadTexture(textureFile);
    s_textures[name] = texture;
    return texture;
}

ModelPtr load2DModel(const std::string& name, const Vertex2DBuffer& vertex2DBuffer)
{
    auto model = load2DModel(vertex2DBuffer);
    s_models[name] = model;
    return model;
}

ModelPtr getModel(const std::string& name)
{
    return s_models.find(name)->second;
}

TexturePtr getTexture(const std::string& name)
{
    return s_textures.find(name)->second;
}

bool doesModelExist(const std::string& name)
{
    return s_models.find(name) != s_models.end();
}

bool doesTextureExist(const std::string& name)
{
    return s_textures.find(name) != s_textures.end();
}

}
}