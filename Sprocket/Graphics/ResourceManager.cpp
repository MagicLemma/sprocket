#include "ResourceManager.h"
#include "Log.h"
#include "ObjParser.h"

#include <filesystem>

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Sprocket {

// Given a vertex buffer and index buffer, load them to VBOs and set vertex attributes
// in the currently bound VAO.
void ResourceManager::loadToVBOs(const Vertex3DBuffer& vertices,
                                 const IndexBuffer& indices)
{
    // Bind a Vertex Buffer
    createVBO(VBOType::VERTEX_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Set Vertex Attributes in the VAO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, position));
                                           
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, texture));

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex3D),
                          (void*)offsetof(Vertex3D, normal));

    // Bind an Index Buffer to the VAO
    createVBO(VBOType::INDEX_BUFFER);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

unsigned int ResourceManager::createVAO()
{
    unsigned int vaoId;
    glGenVertexArrays(1, &vaoId);
    d_vaoList.push_back(vaoId);
    glBindVertexArray(vaoId);
    return vaoId;
}

unsigned int ResourceManager::createVBO(VBOType type)
{
    unsigned int vboId;
    glGenBuffers(1, &vboId);
    d_vboList.push_back(vboId);
    if (type == VBOType::VERTEX_BUFFER) {
        glBindBuffer(GL_ARRAY_BUFFER, vboId);
    } else if (type == VBOType::INDEX_BUFFER) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId);
    }
    return vboId;
}

unsigned int ResourceManager::createTEX(TextureType type)
{
    unsigned int texId;
    glGenTextures(1, &texId);
    d_texList.push_back(texId);
    if (type == TextureType::FLAT) {
        glBindTexture(GL_TEXTURE_2D, texId);
    } else if (type == TextureType::CUBE) {
        glBindTexture(GL_TEXTURE_CUBE_MAP, texId);    
    }
    return texId;
}

unsigned int ResourceManager::createFBO()
{
    unsigned int fboId;
    glGenFramebuffers(1, &fboId);
    d_fboList.push_back(fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    return fboId;
}

unsigned int ResourceManager::createRBO()
{
    unsigned int rboId;
    glGenRenderbuffers(1, &rboId);
    d_rboList.push_back(rboId);
    glBindRenderbuffer(GL_RENDERBUFFER, rboId);
    return rboId;
}

ResourceManager::~ResourceManager()
{
    SPKT_LOG_INFO("Cleaning up VAOs, VBOs, Textures, Frame/Render Buffers");
    glDeleteVertexArrays(d_vaoList.size(), d_vaoList.data());
    glDeleteRenderbuffers(d_rboList.size(), d_rboList.data());
    glDeleteBuffers(d_vboList.size(), d_vboList.data());
    glDeleteFramebuffers(d_fboList.size(), d_fboList.data());
    glDeleteTextures(d_texList.size(), d_texList.data());
}

Model3D ResourceManager::loadModel3D(const std::string& objFile)
{
    std::lock_guard<std::mutex> lock(d_mutex);

    auto it = d_model3Ds.find(objFile);
    if (it != d_model3Ds.end()) {
        SPKT_LOG_INFO("Using cached version of {}", objFile);
        return it->second;
    }

    if (!std::filesystem::exists(objFile)) {
        SPKT_LOG_ERROR("File {} does not exist! Returning empty Model", objFile);
        return Model3D(0, 0);
    }

    // Load new model
    SPKT_LOG_INFO("Loading model '{}'", objFile);
    auto [vertices, indices] = parseObjFile(objFile);
    unsigned int vao = createVAO();
    loadToVBOs(vertices, indices);
    glBindVertexArray(0);
    Model3D newModel(vao, indices.size());

    // Cache the new model
    d_model3Ds.insert(std::make_pair(objFile, newModel));
    SPKT_LOG_TRACE("Cached model '{}'", objFile);
    return newModel;
}

Model3D ResourceManager::loadModel3D(const Vertex3DBuffer& vertices,
                                     const IndexBuffer& indices)
{
    std::lock_guard<std::mutex> lock(d_mutex);

    unsigned int vao = createVAO();
    loadToVBOs(vertices, indices);
    glBindVertexArray(0);
    return Model3D(vao, indices.size());
}

Model2D ResourceManager::loadModel2D(const Vertex2DBuffer& vertices)
{
    std::lock_guard<std::mutex> lock(d_mutex);

    unsigned int vao = createVAO();

    // Bind a Vertex Buffer
    createVBO(VBOType::VERTEX_BUFFER);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex2D) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    // Set Vertex Attributes in the VAO
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),
                          (void*)offsetof(Vertex2D, position));

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),
                          (void*)offsetof(Vertex2D, texture));

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex2D),
                          (void*)offsetof(Vertex2D, colour));

    // Unbind the VAO
    glBindVertexArray(0);

    return Model2D(vao, vertices.size());
}

Texture ResourceManager::loadTexture(const std::string& pngFile)
{
    std::lock_guard<std::mutex> lock(d_mutex);

    auto it = d_textures.find(pngFile);
    if (it != d_textures.end()) {
        SPKT_LOG_INFO("Using cached version of {}", pngFile);
        return it->second;
    }

    if (!std::filesystem::exists(pngFile)) {
        SPKT_LOG_ERROR("File {} does not exist! Returning empty Texture", pngFile);
        return Texture(0, 0, 0, 0);
    }

    unsigned int textureId = createTEX(TextureType::FLAT);

    SPKT_LOG_INFO("Loading texture '{}'", pngFile);
    int width, height, bpp;
    unsigned char* data = stbi_load(pngFile.c_str(), &width, &height, &bpp, 4);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D,
                 0, GL_RGBA8, width, height,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    Texture newTexture(textureId, width, height, bpp);
    d_textures.insert(std::make_pair(pngFile, newTexture));
    SPKT_LOG_TRACE("Cached texture '{}'", pngFile);

    return newTexture;
}

CubeMap ResourceManager::loadCubeMap(const std::array<std::string, 6>& faceFiles)
{
    std::lock_guard<std::mutex> lock(d_mutex);

    unsigned int textureId = createTEX(TextureType::CUBE);

    int width, height, bpp;
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
    
    // Unbind the texture now it is set up.
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return CubeMap(textureId, width, height, bpp);
}

FrameBuffer ResourceManager::loadFrameBuffer(int width, int height, float scale)
{
    std::lock_guard<std::mutex> lock(d_mutex);

    unsigned int fboId = createFBO();

    unsigned int texture = createTEX(TextureType::FLAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width * scale, height * scale, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    // Add depth buffer.
    unsigned int depth = createRBO();
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width * scale, height * scale);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);

    // Validate the framebuffer.
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        SPKT_LOG_ERROR("Created FBO is not complete!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return FrameBuffer(fboId, texture, depth, width, height, scale);
}

}