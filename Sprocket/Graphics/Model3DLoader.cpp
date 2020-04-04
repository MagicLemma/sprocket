#include "Model3DLoader.h"
#include "ObjParser.h"

#include <chrono>

namespace Sprocket {

void Model3DLoader::load(const std::string& objFile)
{
    d_loadedModel = std::async(std::launch::async, [objFile]() -> ModelSpec {
        auto [vertices, indices] = parseObjFile(objFile);
        ModelSpec spec;
        spec.vertices = vertices;
        spec.indices = indices;
        return spec;
    });
}

bool Model3DLoader::completed()
{
    return d_loadedModel.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

Model3D Model3DLoader::get()
{
    auto spec = d_loadedModel.get();
    return Model3D(spec.vertices, spec.indices);
}

}