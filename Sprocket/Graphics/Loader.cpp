#include "Graphics/Loader.h"
#include "Graphics/ObjParser.h"

#include "Graphics/Skyboxes/Skybox.h"
#include "Utility/Log.h"

#include <vector>
#include <fstream>
#include <sstream>
#include <memory>
#include <cstddef>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Sprocket {
namespace Loader {

namespace {

std::vector<unsigned int> s_vaoList;
std::vector<unsigned int> s_vboList;
std::vector<unsigned int> s_texList;

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

void bindVertex3DBuffer(const Vertex3DBuffer& vertex3DBuffer)
{
    unsigned int vboId;
    glGenBuffers(1, &vboId);
    s_vboList.push_back(vboId);

    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * vertex3DBuffer.size(), vertex3DBuffer.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(Vertex3D::posAttr, Vertex3D::posCount, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          reinterpret_cast<void*>(offsetof(Vertex3D, position)));
                                           
    glVertexAttribPointer(Vertex3D::texAttr, Vertex3D::texCount, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          reinterpret_cast<void*>(offsetof(Vertex3D, texture)));

    glVertexAttribPointer(Vertex3D::norAttr, Vertex3D::norCount, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          reinterpret_cast<void*>(offsetof(Vertex3D, normal)));
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

    glVertexAttribPointer(Vertex2D::texAttr, Vertex2D::texCount, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),
                          reinterpret_cast<void*>(offsetof(Vertex2D, texture)));
}

void bindVertexSkyboxBuffer(const VertexSkyboxBuffer& vertexSkyboxBuffer)
{
    unsigned int vboId;
    glGenBuffers(1, &vboId);
    s_vboList.push_back(vboId);

    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexSkybox) * vertexSkyboxBuffer.size(), vertexSkyboxBuffer.data(),
                 GL_STATIC_DRAW);

    glVertexAttribPointer(VertexSkybox::posAttr, VertexSkybox::posCount, GL_FLOAT, GL_FALSE, sizeof(VertexSkybox),
                          reinterpret_cast<void*>(offsetof(VertexSkybox, position)));
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

std::shared_ptr<Model2D> loadModel2D(const Vertex2DBuffer& vertex2DBuffer)
{
    unsigned int vaoId = createVAO();
    bindVertex2DBuffer(vertex2DBuffer);
    unbindVAO();

    return std::make_shared<Model2D>(vaoId, vertex2DBuffer.size());
}

std::shared_ptr<Model3D> loadModel3D(const std::string& objFile)
{
    auto [vertices, indices] = parseObjFile(objFile);
    return loadModel3D(vertices, indices);
}

std::shared_ptr<Model3D> loadModel3D(const Vertex3DBuffer& vertices,
                                     const IndexBuffer& indices)
{
    unsigned int vaoId = createVAO();
    bindVertex3DBuffer(vertices);
    bindIndexBuffer(indices);
    unbindVAO();

    return std::make_shared<Model3D>(vaoId, indices.size());   
}

std::shared_ptr<ModelSkybox> loadSkybox()
{
    auto vertexSkyboxBuffer = getCubeBuffer();

    unsigned int vaoId = createVAO();
    bindVertexSkyboxBuffer(vertexSkyboxBuffer);
    unbindVAO();

    return std::make_shared<ModelSkybox>(vaoId, vertexSkyboxBuffer.size());
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

TextureCubePtr loadCubeMap(const std::array<std::string, 6>& faceFiles)
{
    unsigned int texId;
    glGenTextures(1, &texId);
    s_texList.push_back(texId);

    int width, height, bpp;

    glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

    for (unsigned int i = 0; i != faceFiles.size(); ++i) {
        unsigned char* data = stbi_load(faceFiles[i].c_str(), &width, &height, &bpp, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0, GL_RGB, width, height,
                     0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
    
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return std::make_shared<TextureCube>(texId);
}

}
}