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
    : d_id(id)
    , d_width(width)
    , d_height(height)
    , d_xOffset(xOffset)
    , d_yOffset(yOffset)
    , d_advance(advance)
    , d_model(GetBuffer(textureQuad, width, height))
    , d_textureQuad(textureQuad)
{
}

void Character::Bind() const
{
    d_model.Bind();
}

void Character::Unbind() const
{
    d_model.Unbind();
}

}