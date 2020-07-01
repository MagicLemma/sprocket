#pragma once
#include "Maths.h"
#include "Quad.h"
#include "Model2D.h"

namespace Sprocket {

struct Character
{
    int id; // ASCII ID

    float width;
    float height;
    
    float xOffset;
    float yOffset;
         // Position on the line of the top left of this character

    float advance;
         // How far the cursor should advance after printing this character
 
    Model2D model;
         // Model of the character to load.
 
    Quad textureQuad;
        // Quad that describes the region in the texture atlas that holds
        // this character.

    Character(int id,
              const Quad& textureQuad,
              float width,
              float height,
              float xOffset,
              float yOffset,
              float advance);
};

}