#pragma once
#include "Model3D.h"
#include "CubeMap.h"

#include <memory>

namespace Sprocket {

struct Skybox
{
    std::shared_ptr<Mesh> model;
    CubeMap texture;
};

}