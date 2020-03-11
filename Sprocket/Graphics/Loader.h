#pragma once
#include "Graphics/Modelling/Model2D.h"
#include "Graphics/Modelling/Model3D.h"
#include "Graphics/Texture.h"

#include <vector>
#include <array>
#include <string>
#include <memory>

namespace Sprocket {
namespace Loader {

void init();
void deinit();

enum class VBOType
{
    VERTEX_BUFFER = 0,
    INDEX_BUFFER = 1
};

enum class TextureType
{
    FLAT = 0,
    CUBE = 1
};

// LOW LEVEL LOADERS
// All of these low level loaders create the specified buffer and binds it.
// It is up to the caller of these functions to unbind them.
unsigned int createVAO();
unsigned int createVBO(VBOType type);
unsigned int createTEX(TextureType type);
unsigned int createFBO();
unsigned int createRBO();

// GENERAL MODEL LOADERS
Model2D loadModel2D(const Vertex2DBuffer& buffer);
    // Load a 2D model from a Vertex2DBuffer.

Model3D loadModel3D(const std::string& objFile);
    // Load a 3D model from an OBJ file.

Model3D loadModel3D(const Vertex3DBuffer& vertices,
                    const IndexBuffer& indices);
    // Load a 3D model from a Vertex3DBuffer/IndexBuffer pair.

// TEXTURE LOADERS
Texture loadTexture(const std::string& textureFile);
    // Loads a 2D texture.

}

}