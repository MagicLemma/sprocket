#pragma once
#include "Graphics/Modelling/Model3D.h"
#include "Graphics/Texture.h"
#include "Utility/Maths.h"

namespace Sprocket {

class Entity
{
    Model3D d_model;
    Texture d_texture;

    // Transform Attributes
    Maths::vec3 d_position;
    Maths::vec3 d_rotation;
    float       d_scale;

    Maths::mat4 d_transform;

    void updateTransform();
        // Recalculates the transform matrix.

public:
    Entity(const Model3D& model,
           const Texture& texture,
           const Maths::vec3& position,
           const Maths::vec3& rotation,
           float scale);

    void bind() const;
    void unbind() const;

    // Mutators
    void increasePosition(const Maths::vec3 ds);
    void increasePosition(float dx, float dy, float dz);

    void increaseRotation(const Maths::vec3 dr);
    void increaseRotation(float dx, float dy, float dz);

    // Getters / Setters
    Model3D model() const { return d_model; }
    Texture texture() const { return d_texture; }

    Maths::mat4 transform() const { return d_transform; }

    Maths::vec3 position() const;
    void position(const Maths::vec3& newPosition);

    Maths::vec3 rotation() const;
    void rotation(const Maths::vec3& newRotation);

    float scale() const;
    void scale(float newScale);
};

}