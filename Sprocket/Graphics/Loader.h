#pragma once
#include "Graphics/Model.h"
#include "Graphics/Vertex.h"
#include "Graphics/Texture.h"

#include <vector>
#include <string>

namespace Sprocket {
namespace Loader {

void init();
void deinit();

// Basic loaders. No caching takes place.
Model   loadModel(const std::string& objFile);
Texture loadTexture(const std::string& textureFile);
Model   load2DModel(const Vertex2DBuffer& vertex2DBuffer);

// Cached loaders. Makes the loaded models and textures accessible via
// the get methods below.
Model   loadModel(const std::string& name, const std::string& objFile);
Texture loadTexture(const std::string& name, const std::string& textureFile);
Model   load2DModel(const std::string& name, const Vertex2DBuffer& vertex2DBuffer);

// Get methods for cached models and textures.
Model   getModel(const std::string& name);
Texture getTexture(const std::string& name);

bool doesModelExist(const std::string& name);
bool doesTextureExist(const std::string& name);

}

}