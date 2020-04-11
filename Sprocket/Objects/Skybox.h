#pragma once
#include "Model3D.h"
#include "CubeMap.h"

namespace Sprocket {

struct Skybox
{
    Model3D model;
    CubeMap texture;
};

}