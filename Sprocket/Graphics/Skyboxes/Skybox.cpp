#include "Graphics/Skyboxes/Skybox.h"
#include "Graphics/Loader.h"

namespace Sprocket {

VertexSkyboxBuffer getCubeBuffer()
{
    VertexSkyboxBuffer v{       
    {-1.0f,  1.0f, -1.0f},
    {-1.0f, -1.0f, -1.0f},
    { 1.0f, -1.0f, -1.0f},
    { 1.0f, -1.0f, -1.0f},
     {1.0f,  1.0f, -1.0f},
    {-1.0f,  1.0f, -1.0f},

    {-1.0f, -1.0f,  1.0f},
    {-1.0f, -1.0f, -1.0f},
    {-1.0f,  1.0f, -1.0f},
    {-1.0f,  1.0f, -1.0f},
    {-1.0f,  1.0f,  1.0f},
    {-1.0f, -1.0f,  1.0f},

    {1.0f, -1.0f, -1.0f},
    {1.0f, -1.0f,  1.0f},
    {1.0f,  1.0f,  1.0f},
    {1.0f,  1.0f,  1.0f},
     {1.0f,  1.0f, -1.0f},
     {1.0f, -1.0f, -1.0f},

    {-1.0f, -1.0f,  1.0f},
    {-1.0f,  1.0f,  1.0f},
     {1.0f,  1.0f,  1.0f},
     {1.0f,  1.0f,  1.0f},
     {1.0f, -1.0f,  1.0f},
    {-1.0f, -1.0f,  1.0f},

    {-1.0f,  1.0f, -1.0f},
     {1.0f,  1.0f, -1.0f},
     {1.0f,  1.0f,  1.0f},
     {1.0f,  1.0f,  1.0f},
    {-1.0f,  1.0f,  1.0f},
    {-1.0f,  1.0f, -1.0f},

    {-1.0f, -1.0f, -1.0f},
    {-1.0f, -1.0f,  1.0f},
     {1.0f, -1.0f, -1.0f},
     {1.0f, -1.0f, -1.0f},
    {-1.0f, -1.0f,  1.0f},
     {1.0f, -1.0f,  1.0f}
    };
    for (auto& vertex: v)
    {
        vertex.position[0] = 500 * vertex.position[0];
        vertex.position[1] = 500 * vertex.position[1];
        vertex.position[2] = 500 * vertex.position[2];
    }
    return v;
}

Skybox::Skybox(TextureCubePtr texture)
    : d_texture(texture)
    , d_model(Loader::loadSkybox())
{    
}

void Skybox::bind() const
{
    d_model->bind();
    d_texture->bind();
}

void Skybox::unbind() const
{
    d_texture->unbind();
    d_model->unbind();
}

}