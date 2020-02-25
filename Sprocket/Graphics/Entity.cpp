#include "Graphics/Entity.h"

namespace Sprocket {

Entity::Entity(const Model& model,
               const Texture& texture,
               const Maths::vec3& position,
               const Maths::vec3& rotation,
               float scale)
    : d_model(model)
    , d_texture(texture)
    , d_position(position)
    , d_rotation(rotation)
    , d_scale(scale)
{    
}

void Entity::bind() const
{
    d_model.bind();
    d_texture.bind();
}

void Entity::unbind() const
{
    d_texture.unbind();
    d_model.unbind();
}

void Entity::increasePosition(const Maths::vec3 ds)
{
    d_position += ds;
}

void Entity::increasePosition(float dx, float dy, float dz)
{
    increasePosition(Maths::vec3(dx, dy, dz));
}

void Entity::increaseRotation(const Maths::vec3 dr)
{
    d_rotation += dr;
}

void Entity::increaseRotation(float dx, float dy, float dz)
{
    increaseRotation(Maths::vec3(dx, dy, dz));
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

Maths::vec3 Entity::position() const
{
    return d_position;
}

void Entity::position(const Maths::vec3& newPosition)
{
    d_position = newPosition;
}

Maths::vec3 Entity::rotation() const
{
    return d_rotation;
}

void Entity::rotation(const Maths::vec3& newRotation)
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