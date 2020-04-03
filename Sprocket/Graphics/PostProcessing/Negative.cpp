#include "Negative.h"

namespace Sprocket {

Negative::Negative(ResourceManager* resourceManager, int width, int height)
    : Effect(resourceManager, width, height, 1,
             "Resources/Shaders/Negative.vert",
             "Resources/Shaders/Negative.frag")
{

}
    
}