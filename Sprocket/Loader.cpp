#include "Loader.h"
#include "Vertex.h"
#include "Log.h"
#include "ObjParser.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <cstddef>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Sprocket {

Loader::~Loader()
{
    SPKT_LOG_INFO("Cleaning up VAOs, VBOs and textures");
    glDeleteVertexArrays(d_vaoList.size(), d_vaoList.data());
    glDeleteBuffers(d_vboList.size(), d_vboList.data());
    glDeleteTextures(d_texList.size(), d_texList.data());
}

unsigned int Loader::createVAO()
{
    unsigned int vaoId;
    glGenVertexArrays(1, &vaoId);
    d_vaoList.push_back(vaoId);
    glBindVertexArray(vaoId);
    return vaoId;
}

void Loader::bindVertexBuffer(const std::vector<Vertex>& vertexBuffer)
{
    unsigned int vboId;
    glGenBuffers(1, &vboId);
    d_vboList.push_back(vboId);

    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertexBuffer.size(), vertexBuffer.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(Vertex::posAttr, Vertex::posCount, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, position)));
                                           
    glVertexAttribPointer(Vertex::texAttr, Vertex::texCount, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, texture)));
}

void Loader::bindIndexBuffer(const std::vector<unsigned int>& indexBuffer)
{
    unsigned int vboId;
    glGenBuffers(1, &vboId);
    d_vboList.push_back(vboId);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indexBuffer.size(), indexBuffer.data(),
                 GL_STATIC_DRAW);
}

Model Loader::loadModel(const std::string& objFile)
{
    auto [vertices, indices] = parseObjFile(objFile);

    unsigned int vaoId = createVAO();
    bindVertexBuffer(vertices);
    bindIndexBuffer(indices);
    unbindVAO();

    return Model(vaoId, indices.size());
}

Texture Loader::loadTexture(const std::string& textureFile)
{
    unsigned int texId;
    glGenTextures(1, &texId);
    d_texList.push_back(texId);

    stbi_set_flip_vertically_on_load(1);
    int width, height, bpp;
    unsigned char* data = stbi_load(textureFile.c_str(), &width, &height, &bpp, 4);

    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D,
                 0, GL_RGBA8, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return Texture(texId);
}

void Loader::unbindVAO()
{
    glBindVertexArray(0);
}

}