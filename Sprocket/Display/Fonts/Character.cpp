#include "Character.h"
#include "Log.h"

namespace Sprocket {

Vertex2DBuffer GetBuffer(const Texture& atlas,
                         const Maths::vec2& texTopLeft,
                         double width,
                         double height)
{
    return Vertex2DBuffer{
        {Maths::vec2{0.0f, 0.0f}, Maths::vec2{texTopLeft.x/atlas.Width(), texTopLeft.y/atlas.Height()}},
        {Maths::vec2{width, 0.0f}, Maths::vec2{(texTopLeft.x + width)/atlas.Width(), texTopLeft.y/atlas.Height()}},
        {Maths::vec2{0.0f, height}, Maths::vec2{texTopLeft.x/atlas.Width(), (texTopLeft.y + height)/atlas.Height()}},
        {Maths::vec2{width, height}, Maths::vec2{(texTopLeft.x + width)/atlas.Width(), (texTopLeft.y + height)/atlas.Height()}}       
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