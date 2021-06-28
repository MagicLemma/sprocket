#include "Negative.h"

namespace spkt {

Negative::Negative(int width, int height)
    : Effect(width, height,
             "Resources/Shaders/Negative.vert",
             "Resources/Shaders/Negative.frag")
{

}
    
}