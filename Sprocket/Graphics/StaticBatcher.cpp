#include "StaticBatcher.h"

namespace Sprocket
{

StaticBatcher::StaticBatcher(const std::string& objFile)
    : d_numModels(0)
{
    auto [vertices, indices] = parseObjFile(objFile);
    d_modelVertices = vertices;
    d_modelIndices = indices;
}

void StaticBatcher::addTransform(const Maths::vec3& position,
                                 const Maths::vec3& rotation,
                                 float scale,
                                 float textureIndex)
{
    Maths::mat4 transform = Maths::transform(position, rotation, scale);
    for (const auto& vertex : d_modelVertices) {
        Vertex3D v = vertex;
        Maths::vec4 p = {v.position.x, v.position.y, v.position.z, 1.0};
        v.position = Maths::vec3(transform * p);
        v.textureIndex = textureIndex;
        d_batchedVertices.push_back(v);
    }

    for (const auto& index : d_modelIndices) {
        d_batchedIndices.push_back(index + d_modelVertices.size() * d_numModels);
    }

    ++d_numModels;
}

Model3D StaticBatcher::getModel3D()
{
    return Model3D(d_batchedVertices, d_batchedIndices);
}

}