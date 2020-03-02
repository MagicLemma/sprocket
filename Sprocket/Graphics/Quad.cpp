#include "Quad.h"

namespace Sprocket {

Quad::Quad(
    const Model& model,
    const Maths::vec2& centrePosition,
    float width,
    float height,
    Window* window)
    : d_model(model)
    , d_centrePosition(centrePosition)
    , d_width(width)
    , d_height(height)
    , d_window(window)
{

}
    
}