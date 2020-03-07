#include "Contrast.h"

namespace Sprocket {

Contrast::Contrast(int width, int height)
    : Effect(width, height, 1, "Resources/Shaders/Contrast.vert",
                               "Resources/Shaders/Contrast.frag")
{

}
    
}