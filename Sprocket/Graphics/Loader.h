#pragma once
#include "Graphics/Modelling/Model2D.h"
#include "Graphics/Modelling/Model3D.h"
#include "Graphics/Modelling/ModelSkybox.h"
#include "Graphics/Texture.h"

#include <vector>
#include <array>
#include <string>
#include <memory>

namespace Sprocket {
namespace Loader {

void init();
void deinit();

// GENERAL MODEL LOADERS
std::shared_ptr<Model2D> loadModel2D(const Vertex2DBuffer& vertex2DBuffer);
    // Load a 2D model from a Vertex2DBuffer.

std::shared_ptr<Model3D> loadModel3D(const std::string& objFile);
    // Load a 3D model from an OBJ file.

std::shared_ptr<Model3D> loadModel3D(const Vertex3DBuffer& vertices,
                                     const IndexBuffer& indices);
    // Load a 3D model from a Vertex3DBuffer/IndexBuffer pair.

// SPECIAL MODEL LOADERS
std::shared_ptr<ModelSkybox> loadSkybox();
    // Load a 3D model cube to be used as a skybox. A ModelSkybox object,
    // unlike the general Model3D, has no texture or normal vector data.

// TEXTURE LOADERS
TexturePtr loadTexture(const std::string& textureFile);
    // Loads a 2D texture.
TextureCubePtr loadCubeMap(const std::array<std::string, 6>& faceFiles);
    // Loads a 3D cube map texture.

}

}