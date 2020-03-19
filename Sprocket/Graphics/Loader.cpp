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

}
}