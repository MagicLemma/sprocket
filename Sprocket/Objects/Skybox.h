#pragma once
#include "Model3D.h"
#include "CubeMap.h"

#include <memory>

namespace Sprocket {

struct Skybox
{
    std::shared_ptr<Model3D> model;
    CubeMap texture;
};

}