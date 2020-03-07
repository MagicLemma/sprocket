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
std::vector<unsigned int> s_rboList;
std::vector<unsigned int> s_vboList;
std::vector<unsigned int> s_fboList;
std::vector<unsigned int> s_texList;

}

unsigned int createVAO()
{
    unsigned int vaoId;
    glGenVertexArrays(1, &vaoId);
    s_vaoList.push_back(vaoId);
    glBindVertexArray(vaoId);
    return vaoId;
}

unsigned int createVBO(VBOType type)
{
    unsigned int vboId;
    glGenBuffers(1, &vboId);
    s_vboList.push_back(vboId);
    if (type == VBOType::VERTEX_BUFFER) {
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
    } else if (type == VBOType::INDEX_BUFFER) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId);
    }
    return vboId;
}

unsigned int createTEX(TextureType type)
{
    unsigned int texId;
    glGenTextures(1, &texId);
    s_texList.push_back(texId);
    if (type == TextureType::FLAT) {
        glBindTexture(GL_TEXTURE_2D, texId);
    } else if (type == TextureType::CUBE) {
        glBindTexture(GL_TEXTURE_CUBE_MAP, texId);    
    }
    return texId;
}

unsigned int createFBO()
{
    unsigned int fboId;
    glGenFramebuffers(1, &fboId);
    s_fboList.push_back(fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    return fboId;
}

unsigned int createRBO()
{
    unsigned int rboId;
    glGenRenderbuffers(1, &rboId);
    s_rboList.push_back(rboId);
    glBindRenderbuffer(GL_RENDERBUFFER, rboId);
    return rboId;
}

void bindVertex2DBuffer(const Vertex2DBuffer& buffer)
{
    createVBO(VBOType::VERTEX_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * buffer.size(), buffer.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),
                          (void*)offsetof(Vertex2D, position));

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),
                          (void*)offsetof(Vertex2D, texture));
}

void bindVertex3DBuffer(const Vertex3DBuffer& buffer)
{
    createVBO(VBOType::VERTEX_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * buffer.size(), buffer.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, position));
                                           
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, texture));

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, normal));
}

void bindIndexBuffer(const IndexBuffer& buffer)
{
    createVBO(VBOType::INDEX_BUFFER);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * buffer.size(), buffer.data(), GL_STATIC_DRAW);
}

void init()
{
    SPKT_LOG_INFO("Initialised Loader");
}

void deinit()
{
    SPKT_LOG_INFO("Deinitialised Loader: Cleaning up VAOs, VBOs, FBOs, RBOs and textures");
    glDeleteVertexArrays(s_vaoList.size(), s_vaoList.data());
    glDeleteRenderbuffers(s_rboList.size(), s_rboList.data());
    glDeleteBuffers(s_vboList.size(), s_vboList.data());
    glDeleteFramebuffers(s_fboList.size(), s_fboList.data());
    glDeleteTextures(s_texList.size(), s_texList.data());
}

std::shared_ptr<Model2D> loadModel2D(const Vertex2DBuffer& vertex2DBuffer)
{
    unsigned int vaoId = createVAO();
    bindVertex2DBuffer(vertex2DBuffer);
    glBindVertexArray(0);

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
    glBindVertexArray(0);

    return std::make_shared<Model3D>(vaoId, indices.size());   
}

TexturePtr loadTexture(const std::string& textureFile)
{
    unsigned int texId = createTEX(TextureType::FLAT);

    stbi_set_flip_vertically_on_load(1);
    int width, height, bpp;
    unsigned char* data = stbi_load(textureFile.c_str(), &width, &height, &bpp, 4);
;
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

}
}