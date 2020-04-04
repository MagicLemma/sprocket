#pragma once
#include "Model3D.h"
#include "Material.h"
#include "Maths.h"
#include "Model3DLoader.h"

namespace Sprocket {

class Entity
{
    Model3DLoader d_modelLoader;
    Model3D d_model;
    bool d_loading;
    Material d_material;

    // Transform Attributes
    Maths::vec3 d_position;
    Maths::vec3 d_rotation;
    float       d_scale;

    Maths::mat4 d_transform;

    void updateTransform();
        // Recalculates the transform matrix.

public:
    Entity(const std::string& model,
           const Material& material,
           const Maths::vec3& position,
           const Maths::vec3& rotation,
           float scale);

    void update();

    void bind() const;
    void unbind() const;

    // Mutators
    void increasePosition(const Maths::vec3 ds);
    void increasePosition(float dx, float dy, float dz);

    void increaseRotation(const Maths::vec3 dr);
    void increaseRotation(float dx, float dy, float dz);

    // Getters / Setters
    Model3D model() const { return d_model; }
    Material material() const { return d_material; }

    Maths::mat4 transform() const { return d_transform; }

    Maths::vec3 position() const;
    void position(const Maths::vec3& newPosition);

    Maths::vec3 rotation() const;
    void rotation(const Maths::vec3& newRotation);

    float scale() const;
    void scale(float newScale);
};

}