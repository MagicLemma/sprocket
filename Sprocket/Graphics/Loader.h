#pragma once
#include "Graphics/Model.h"
#include "Graphics/Vertex.h"
#include "Graphics/Texture.h"

#include <vector>
#include <string>
#include <memory>

namespace Sprocket {
namespace Loader {

void init();
void deinit();

// Basic loaders. No caching takes place.
ModelPtr   loadModel(const std::string& objFile);
TexturePtr loadTexture(const std::string& textureFile);
ModelPtr   load2DModel(const Vertex2DBuffer& vertex2DBuffer);

// Cached loaders. Makes the loaded models and textures accessible via
// the get methods below.
ModelPtr   loadModel(const std::string& name, const std::string& objFile);
TexturePtr loadTexture(const std::string& name, const std::string& textureFile);
ModelPtr   load2DModel(const std::string& name, const Vertex2DBuffer& vertex2DBuffer);

// Get methods for cached models and textures.
ModelPtr   getModel(const std::string& name);
TexturePtr getTexture(const std::string& name);

bool doesModelExist(const std::string& name);
bool doesTextureExist(const std::string& name);

}

}