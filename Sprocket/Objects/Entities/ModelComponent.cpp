#include "ModelComponent.h"

namespace Sprocket {

ModelComponent::ModelComponent(const Model3D& model,
                               const Material& material)
    : d_model(model)
    , d_materials({material})
{   
}

ModelComponent::ModelComponent(const Model3D& model,
                               const std::vector<Material>& materials)
    : d_model(model)
    , d_materials(materials)
{   
}

ModelComponent::ModelComponent()
    : d_model()
    , d_materials()
{   
}

bool ModelComponent::operator==(const ModelComponent& other) const
{
    return d_model == other.d_model && d_materials == other.d_materials;
}

}