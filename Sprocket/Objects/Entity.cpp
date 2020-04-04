#include "Entity.h"

namespace Sprocket {

Entity::Entity(const std::string& model,
               const Material& material,
               const Maths::vec3& position,
               const Maths::vec3& rotation,
               float scale)
    : d_modelLoader()
    , d_model()
    , d_loading(true)
    , d_material(material)
    , d_position(position)
    , d_rotation(rotation)
    , d_scale(scale)
    , d_transform(Maths::transform(position, rotation, scale))
{   
    d_modelLoader.load(model);
}

void Entity::updateTransform()
{
    d_transform = Maths::transform(d_position, d_rotation, d_scale);
}

void Entity::bind() const
{
    d_model.bind();
    d_material.bind();
}

void Entity::unbind() const
{
    d_material.unbind();
    d_model.unbind();
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

void Entity::update()
{
    if (d_loading && d_modelLoader.completed()) {
        d_model = d_modelLoader.get();
        d_loading = false;
    }
}

}