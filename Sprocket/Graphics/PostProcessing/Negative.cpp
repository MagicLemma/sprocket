#include "Negative.h"

namespace Sprocket {

Negative::Negative(int width, int height)
    : Effect(width, height,
             "Resources/Shaders/Negative.vert",
             "Resources/Shaders/Negative.frag")
{

}
    
}