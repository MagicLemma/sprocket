#include "Negative.h"

namespace Sprocket {

Negative::Negative(int width, int height)
    : Effect(width, height, 1, "Resources/Shaders/Negative.vert",
                               "Resources/Shaders/Negative.frag")
{

}
    
}