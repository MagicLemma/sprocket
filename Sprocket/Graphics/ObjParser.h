#pragma once
#include "Graphics/Vertex.h"

#include <utility>
#include <string>

namespace Sprocket {

std::pair<VertexBuffer, IndexBuffer> parseObjFile(const std::string& objFile);

}
