#include "Skybox.h"
#include "Loader.h"

#include <glad/glad.h>
#include <stb_image.h>

namespace Sprocket {
namespace {

std::array<float, 108> getCubeBuffer()
{   
    std::array<float, 108> v{       
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    for (auto& x : v) { x *= 500; }
    return v;
}

}

Skybox::Skybox(const std::array<std::string, 6>& faceFiles)
    : d_texId(0)
    , d_vaoId(0)
{   
    // New VAO to store the model in.
    d_vaoId = Loader::createVAO();

    // Create the cube data and add to the VAO.
    auto vertexSkyboxBuffer = getCubeBuffer();
    createVBO(Loader::VBOType::VERTEX_BUFFER);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexSkyboxBuffer.size(),
                 vertexSkyboxBuffer.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), reinterpret_cast<void*>(0));

    // Unbind the VAO now we have set it up.
    glBindVertexArray(0);

    // Next, create the cube map texture.
    d_texId = Loader::createTEX(Loader::TextureType::CUBE);

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
}

void Skybox::bind() const
{
    // Bind the model.
    glBindVertexArray(d_vaoId);
    glEnableVertexAttribArray(0);

    // Bind the cube map.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, d_texId);
}

void Skybox::unbind() const
{
    // Unbind the texture.
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    // Unbind the model.
    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
}

}