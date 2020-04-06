#include "ModelComponent.h"

namespace Sprocket {

ModelComponent::ModelComponent(const Model3D& model,
                               const Material& material)
    : d_model(model)
    , d_material(material)
{   
}

ModelComponent::ModelComponent()
    : d_model()
    , d_material()
{   
}

}