#pragma once
#include "Component.h"
#include "Maths.h"

namespace Sprocket {

class PositionComponent : public Component
{
    Maths::vec3 d_position;
    Maths::vec3 d_rotation;
    float       d_scale;

    Maths::mat4 d_transform;

public:
    PositionComponent(const Maths::vec3& position,
                      const Maths::vec3& rotation,
                      float scale);

    PositionComponent();

    Maths::mat4 transform() const { return d_transform; }

    Maths::vec3 position() const;
    void position(const Maths::vec3& newPosition);

    Maths::vec3 rotation() const;
    void rotation(const Maths::vec3& newRotation);

    float scale() const;
    void scale(float newScale);
};

}