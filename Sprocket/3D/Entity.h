#pragma once
#include "3D/Model.h"
#include "3D/Texture.h"
#include "Utility/Maths.h"

namespace Sprocket {

class Entity
{
    Model       d_model;
    Texture     d_texture;
    Maths::vec3 d_position;
    Maths::vec3 d_rotation;
    float       d_scale;

public:
    Entity(const Model& model,
           const Texture& texture,
           const Maths::vec3& position,
           const Maths::vec3& rotation,
           float scale);

    void bind() const;
    void unbind() const;

    void increasePosition(const Maths::vec3 ds);
    void increasePosition(float dx, float dy, float dz);

    void increaseRotation(const Maths::vec3 dr);
    void increaseRotation(float dx, float dy, float dz);

    // Getters / Setters
    Model model() const;
    void model(const Model& newModel);

    Texture texture() const;
    void texture(const Texture& newTexture);

    Maths::vec3 position() const;
    void position(const Maths::vec3& newPosition);

    Maths::vec3 rotation() const;
    void rotation(const Maths::vec3& newRotation);

    float scale() const;
    void scale(float newScale);
};

}