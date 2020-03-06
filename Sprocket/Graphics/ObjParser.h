#pragma once
#include "Graphics/Modelling/Model3D.h"

#include <utility>
#include <string>

namespace Sprocket {

std::pair<Vertex3DBuffer, IndexBuffer> parseObjFile(const std::string& objFile);

}
