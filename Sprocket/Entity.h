#pragma once
#include "Model.h"
#include "Texture.h"

#include <glm/glm.hpp>

namespace Sprocket {

class Entity
{
    Model     d_model;
    Texture   d_texture;
    glm::vec3 d_position;
    glm::vec3 d_rotation;
    float     d_scale;

public:
    Entity(const Model& model,
           const Texture& texture,
           const glm::vec3& position,
           const glm::vec3& rotation,
           float scale);

    void increasePosition(const glm::vec3 ds);
    void increasePosition(float dx, float dy, float dz);

    void increaseRotation(const glm::vec3 dr);
    void increaseRotation(float dx, float dy, float dz);

    // Getters / Setters
    Model model() const;
    void model(const Model& newModel);

    Texture texture() const;
    void texture(const Texture& newTexture);

    glm::vec3 position() const;
    void position(const glm::vec3& newPosition);

    glm::vec3 rotation() const;
    void rotation(const glm::vec3& newRotation);

    float scale() const;
    void scale(float newScale);
};

}