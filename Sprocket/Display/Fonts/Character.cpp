#include "Character.h"
#include "Log.h"

namespace Sprocket {

Vertex2DBuffer GetBuffer(const Quad& textureQuad,
                         double width,
                         double height)
{
    auto x = textureQuad.position.x;
    auto y = textureQuad.position.y;
    auto w = textureQuad.width;
    auto h = textureQuad.height;
    return Vertex2DBuffer{
        {{0.0f,  0.0f  }, {x,     y    }},
        {{width, 0.0f  }, {x + w, y    }},
        {{0.0f,  height}, {x,     y + h}},
        {{width, height}, {x + w, y + h}}      
    };
}
    
Character::Character(int id,
                     const Quad& textureQuad,
                     float width,
                     float height,
                     float xOffset,
                     float yOffset,
                     float advance)
    : id(id)
    , width(width)
    , height(height)
    , xOffset(xOffset)
    , yOffset(yOffset)
    , advance(advance)
    , model(GetBuffer(textureQuad, width, height))
    , textureQuad(textureQuad)
{
}

}