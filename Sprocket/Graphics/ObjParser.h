#pragma once
#include "Model3D.h"

#include <utility>
#include <string>

namespace Sprocket {

std::pair<Vertex3DBuffer, IndexBuffer> parseObjFile(const std::string& objFile);

}
