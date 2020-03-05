#pragma once
#include <memory>

namespace Sprocket {

class Texture
{
    unsigned int d_textureId;

    // Properties for specular lighting.
    float        d_shineDamper;
    float        d_reflectivity;

public:
    Texture(unsigned int textureId);

    void bind() const;
    void unbind() const;

    float shineDamper() const { return d_shineDamper; }
    void shineDamper(float newShineDamper) { d_shineDamper = newShineDamper; }

    float reflectivity() const { return d_reflectivity; }
    void reflectivity(float newReflectivity) { d_reflectivity = newReflectivity; } 
};

class TextureCube
{
    unsigned int d_textureId;

public:
    TextureCube(unsigned int textureId);

    void bind() const;
    void unbind() const;
};

using TexturePtr = std::shared_ptr<Texture>;
using TextureCubePtr = std::shared_ptr<TextureCube>;

}