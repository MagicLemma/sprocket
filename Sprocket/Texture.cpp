#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Sprocket {

Texture::Texture(const std::string& path)
    : d_filePath(path)
    , d_textureData(nullptr)
    , d_width(0)
    , d_height(0)
    , d_bpp(0)
{
    stbi_set_flip_vertically_on_load(1);
    d_textureData = stbi_load(path.c_str(), &d_width, &d_height, &d_bpp, 4);
}

Texture::~Texture()
{
    stbi_image_free(d_textureData);
}

}