#pragma once
#include "Lens.h"
#include "Maths.h"
#include "Window.h"

namespace Sprocket {
    
class OrthographicLens : public Lens
{
    float d_width;
    float d_height;
    float d_length;

    Maths::mat4 d_projection;

public:
    OrthographicLens(float width, float height, float length);

    Maths::mat4 Projection() const override;
        // Returns a projection matrix.

    void SetWidth(float width);
    void SetHeight(float height);
    void SetLength(float length);
};

}