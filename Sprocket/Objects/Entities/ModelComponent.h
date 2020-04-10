#pragma once
#include "Component.h"
#include "Model3D.h"
#include "Material.h"

#include <vector>

namespace Sprocket {

class ModelComponent : public Component
{
    Model3D d_model;
    std::vector<Material> d_materials;

public:
    ModelComponent(const Model3D& model,
                   const Material& material);

    ModelComponent(const Model3D& model,
                   const std::vector<Material>& materials);

    ModelComponent();

    Model3D model() { return d_model; }
    void model(const Model3D& model) { d_model = model; }

    Material material(std::size_t index = 0) { return d_materials[index]; }
    void addMaterial(const Material& material) { d_materials.push_back(material); }

    bool operator==(const ModelComponent& other) const;
};

}