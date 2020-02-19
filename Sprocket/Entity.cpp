#include "Entity.h"

namespace Sprocket {

Entity::Entity(const Model& model,
               const Texture& texture,
               const glm::vec3& position,
               const glm::vec3& rotation,
               float scale)
    : d_model(model)
    , d_texture(texture)
    , d_position(position)
    , d_rotation(rotation)
    , d_scale(scale)
{    
}

void Entity::increasePosition(const glm::vec3 ds)
{
    d_position += ds;
}

void Entity::increasePosition(float dx, float dy, float dz)
{
    increasePosition(glm::vec3(dx, dy, dz));
}

void Entity::increaseRotation(const glm::vec3 dr)
{
    d_rotation += dr;
}

void Entity::increaseRotation(float dx, float dy, float dz)
{
    increaseRotation(glm::vec3(dx, dy, dz));
}


Model Entity::model() const
{ 
    return d_model;
}

void Entity::model(const Model& newModel)
{
    d_model = newModel;
}

Texture Entity::texture() const {
    return d_texture;
}

void Entity::texture(const Texture& newTexture)
{
    d_texture = newTexture;
}

glm::vec3 Entity::position() const
{
    return d_position;
}

void Entity::position(const glm::vec3& newPosition)
{
    d_position = newPosition;
}

glm::vec3 Entity::rotation() const
{
    return d_rotation;
}

void Entity::rotation(const glm::vec3& newRotation)
{
    d_rotation = newRotation;
}

float Entity::scale() const
{
    return d_scale;
}

void Entity::scale(float newScale)
{
    d_scale = newScale;
}

}