#pragma once
#include "Graphics/Model.h"
#include "Graphics/Vertex.h"
#include "Graphics/Texture.h"

#include <vector>
#include <array>
#include <string>
#include <memory>

namespace Sprocket {
namespace Loader {

void init();
void deinit();

// MODEL LOADERS
ModelPtr   loadModel(const std::string& objFile);
    // Load a 3D Model from a file.
ModelPtr   loadBuffers(const VertexBuffer& vertices, const IndexBuffer& indices);
    // Load a 3D Model from a VertexBuffer/IndexBuffer pair.
ModelPtr   load2DModel(const Vertex2DBuffer& vertex2DBuffer);
    // Load a 2D Model from a Vertex2DBuffer. Vertices of these models only
    // have two position parameters and no normal vectors.
ModelPtr   loadCube(const VertexSkyboxBuffer& vertexSkyboxBuffer);

// TEXTURE LOADERS
TexturePtr loadTexture(const std::string& textureFile);
    // Loads a 2D texture.
TextureCubePtr loadCubeMap(const std::array<std::string, 6>& faceFiles);
    // Loads a 3D cube map texture.

}

}