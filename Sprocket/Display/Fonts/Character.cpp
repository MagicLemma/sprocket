#include "Character.h"
#include "Log.h"

namespace Sprocket {

Vertex2DBuffer getBuffer(const Texture& atlas,
                         const Maths::vec2& texTopLeft,
                         double width,
                         double height)
{
    return Vertex2DBuffer{
        {Maths::vec2{0.0f, 0.0f}, Maths::vec2{texTopLeft.x/atlas.width(), texTopLeft.y/atlas.height()}},
        {Maths::vec2{width, 0.0f}, Maths::vec2{(texTopLeft.x + width)/atlas.width(), texTopLeft.y/atlas.height()}},
        {Maths::vec2{0.0f, height}, Maths::vec2{texTopLeft.x/atlas.width(), (texTopLeft.y + height)/atlas.height()}},
        {Maths::vec2{width, height}, Maths::vec2{(texTopLeft.x + width)/atlas.width(), (texTopLeft.y + height)/atlas.height()}}       
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
    , d_model(getBuffer(atlas, texTopLeft, width, height))
    , d_atlas(atlas)
{
}

void Character::bind() const
{
    d_atlas.bind();
    d_model.bind();
}

void Character::unbind() const
{
    d_model.unbind();
    d_atlas.unbind();
}

}