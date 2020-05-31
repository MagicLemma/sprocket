#include "Image.h"
#include "MouseEvent.h"

namespace Sprocket {

Image::Image(const Texture& texture)
    : Widget((float)texture.Width(), (float)texture.Height())
{
    d_base.texture = texture;
}

}