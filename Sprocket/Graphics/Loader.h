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

Model   loadModel(const std::string& objFile);
Texture loadTexture(const std::string& textureFile);
Model   load2DModel(const Vertex2DBuffer& vertex2DBuffer);

}

}