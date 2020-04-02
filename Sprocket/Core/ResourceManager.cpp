#include "ResourceManager.h"
#include "Log.h"

#include <filesystem>

namespace Sprocket {

Model3D ResourceManager::loadModel3D(const std::string& name,
                                     const std::string& objFile)
{
    if (!std::filesystem::exists(objFile)) {
        SPKT_LOG_ERROR("File {} does not exist!", objFile);
    }

    auto model = d_models.insert(std::make_pair(name, Model3D(objFile)));

    if (model.second == false) {
        SPKT_LOG_ERROR("Model with name {} already exists! Entity may look wrong!", name);
    }

    return model.first->second;
}

Model3D ResourceManager::getModel3D(const std::string& name) const
{
    auto it = d_models.find(name);
    if (it == d_models.end()) {
        SPKT_LOG_ERROR("No model with name {} exists!", name);
        throw std::runtime_error("No Model with the specified name exists!");
    }

    return it->second;
}

}