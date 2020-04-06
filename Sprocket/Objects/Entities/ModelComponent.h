#pragma once
#include "Component.h"
#include "Model3D.h"
#include "Material.h"

namespace Sprocket {

class ModelComponent : public Component
{
    Model3D d_model;
    Material d_material;

public:
    ModelComponent(const Model3D& model,
                   const Material& material);

    ModelComponent();

    Model3D model() { return d_model; }
    void model(const Model3D& model) { d_model = model; }

    Material material() { return d_material; }
    void material(const Material& material) { d_material = material; }
};

}