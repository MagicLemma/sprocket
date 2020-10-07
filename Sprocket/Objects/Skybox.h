#pragma once
#include "Mesh.h"
#include "CubeMap.h"

#include <memory>

namespace Sprocket {

struct Skybox
{
    std::shared_ptr<Mesh> model;
    CubeMap texture;
};

}