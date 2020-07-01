#include "Character.h"
#include "Log.h"

namespace Sprocket {

Vertex2DBuffer GetBuffer(const Texture& atlas,
                         const Maths::vec2& texTopLeft,
                         double width,
                         double height)
{
    auto x = texTopLeft.x;
    auto y = texTopLeft.y;
    auto aw = atlas.Width();
    auto ah = atlas.Height();
    return Vertex2DBuffer{
        {{0.0f, 0.0f}, {x/aw, y/ah}},
        {{width, 0.0f}, {(x + width)/aw, y/ah}},
        {{0.0f, height}, {x/aw, (y + height)/ah}},
        {{width, height}, {(x + width)/aw, (y + height)/ah}}       
    };
}
    
Character::Character(const Texture& atlas,
                     int id,
                     const Maths::vec2& texTopLeft,
                     float width,
                     float height,
                     float xOffset,
                     float yOffset,
                     float advance)
    : d_id(id)
    , d_atlasQuad({texTopLeft, width, height})
    , d_xOffset(xOffset)
    , d_yOffset(yOffset)
    , d_advance(advance)
    , d_model(GetBuffer(atlas, texTopLeft, width, height))
    , d_atlas(atlas)
{
}

void Character::Bind() const
{
    d_atlas.Bind();
    d_model.Bind();
}

void Character::Unbind() const
{
    d_model.Unbind();
    d_atlas.Unbind();
}

}