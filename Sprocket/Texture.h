#pragma once

namespace Sprocket {

class Texture
{
    unsigned int d_textureId;

public:
    Texture(unsigned int textureId);

    void bind() const;
    void unbind() const;
};

}