#include "PositionComponent.h"

namespace Sprocket {

PositionComponent::PositionComponent(const Maths::vec3& position,
                                     const Maths::vec3& rotation,
                                     float scale)
    : d_position(position)
    , d_rotation(rotation)
    , d_scale(scale)
    , d_transform(Maths::transform(d_position, d_rotation, d_scale))
{   
}

PositionComponent::PositionComponent()
    : d_position({0.0, 0.0, 0.0})
    , d_rotation({0.0, 0.0, 0.0})
    , d_scale(1.0f)
    , d_transform(Maths::transform(d_position, d_rotation, d_scale))
{   
}


Maths::vec3 PositionComponent::position() const
{
    return d_position;
}

void PositionComponent::position(const Maths::vec3& newPosition)
{
    d_position = newPosition;
    d_transform = Maths::transform(d_position, d_rotation, d_scale);
}

Maths::vec3 PositionComponent::rotation() const
{
    return d_rotation;
}

void PositionComponent::rotation(const Maths::vec3& newRotation)
{
    d_rotation = newRotation;
    d_transform = Maths::transform(d_position, d_rotation, d_scale);
}

float PositionComponent::scale() const
{
    return d_scale;
}

void PositionComponent::scale(float newScale)
{
    d_scale = newScale;
    d_transform = Maths::transform(d_position, d_rotation, d_scale);
}

}