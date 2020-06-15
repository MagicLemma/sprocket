#include "OrthographicLens.h"
#include "WindowEvent.h"
#include "Log.h"

namespace Sprocket {

OrthographicLens::OrthographicLens(float width, float height, float length)
    : d_width(width)
    , d_height(height)
    , d_length(length)
{
    d_projection = Maths::Ortho(width, height, length);
}

Maths::mat4 OrthographicLens::Projection() const
{
    return d_projection;
}

void OrthographicLens::SetWidth(float width)
{
    d_width = width;
    d_projection = Maths::Ortho(d_width, d_height, d_length);
}

void OrthographicLens::SetHeight(float height)
{
    d_height = height;
    d_projection = Maths::Ortho(d_width, d_height, d_length);
}

void OrthographicLens::SetLength(float length)
{
    d_length = length;
    d_projection = Maths::Ortho(d_width, d_height, d_length);
}
    
}