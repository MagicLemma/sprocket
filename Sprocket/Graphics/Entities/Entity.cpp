#include "Graphics/Entities/Entity.h"

namespace Sprocket {

Entity::Entity(std::shared_ptr<Model3D> model,
               TexturePtr texture,
               const Maths::vec3& position,
               const Maths::vec3& rotation,
               float scale)
    : d_model(model)
    , d_texture(texture)
    , d_position(position)
    , d_rotation(rotation)
    , d_scale(scale)
    , d_transform(Maths::createTransformationMatrix(position, rotation, scale))
{    
}

void Entity::updateTransform()
{
    d_transform = Maths::createTransformationMatrix(
        d_position,
        d_rotation,
        d_scale
    );
}

void Entity::bind() const
{
    d_model->bind();
    d_texture->bind();
}

void Entity::unbind() const
{
    d_texture->unbind();
    d_model->unbind();
}

void Entity::increasePosition(const Maths::vec3 ds)
{
    d_position += ds;
    updateTransform();
}

void Entity::increasePosition(float dx, float dy, float dz)
{
    increasePosition(Maths::vec3(dx, dy, dz));
}

void Entity::increaseRotation(const Maths::vec3 dr)
{
    d_rotation += dr;
    updateTransform();
}

void Entity::increaseRotation(float dx, float dy, float dz)
{
    increaseRotation(Maths::vec3(dx, dy, dz));
}

Maths::vec3 Entity::position() const
{
    return d_position;
}

void Entity::position(const Maths::vec3& newPosition)
{
    d_position = newPosition;
    updateTransform();
}

Maths::vec3 Entity::rotation() const
{
    return d_rotation;
}

void Entity::rotation(const Maths::vec3& newRotation)
{
    d_rotation = newRotation;
    updateTransform();
}

float Entity::scale() const
{
    return d_scale;
}

void Entity::scale(float newScale)
{
    d_scale = newScale;
    updateTransform();
}

}