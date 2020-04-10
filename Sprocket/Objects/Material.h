#pragma once
#include "Texture.h"

namespace Sprocket {
    
class Material
{
    Texture d_texture;

    // Properties for specular lighting.
    float        d_shineDamper;
    float        d_reflectivity;

public:
    Material(const Texture& texture);
    Material();

    void bind(int index = 0) const;
    void unbind() const;

    float shineDamper() const { return d_shineDamper; }
    void shineDamper(float newShineDamper) { d_shineDamper = newShineDamper; }

    float reflectivity() const { return d_reflectivity; }
    void reflectivity(float newReflectivity) { d_reflectivity = newReflectivity; } 

    bool operator==(const Material& other) const;
};

}