#include "Image.h"

namespace Sprocket {

Image::Image(const std::string& pngFile, const Maths::vec2& topLeft)
    : d_texture(pngFile)
    , d_quad(topLeft, (float)d_texture.width(), (float)d_texture.height())
{

}

Image::Image(const Texture& texture, const Maths::vec2& topLeft)
    : d_texture(texture)
    , d_quad(topLeft, (float)d_texture.width(), (float)d_texture.height())
{

}

}