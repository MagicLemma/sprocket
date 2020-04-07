#pragma once
#include "Model3D.h"
#include "ObjParser.h"
#include "Maths.h"

#include <string>

namespace Sprocket {

class StaticBatcher
{
    Vertex3DBuffer d_modelVertices;
    IndexBuffer    d_modelIndices;

    Vertex3DBuffer d_batchedVertices;
    IndexBuffer    d_batchedIndices;

    unsigned int d_numModels;

public:
    StaticBatcher(const std::string& objFile);

    void addTransform(const Maths::vec3& position, const Maths::vec3& rotation, float scale);

    Model3D getModel3D();
};

}