#include "Image.h"
#include "MouseEvent.h"

namespace Sprocket {

Image::Image(const Texture& texture)
    : Widget((float)texture.width(), (float)texture.height())
{
    d_base.texture = texture;
}

}