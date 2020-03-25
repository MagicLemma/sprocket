#include "Font.h"

namespace Sprocket {

Font::Font(const CharacterMap& characters, const Texture& atlas)
    : d_characters(characters)
    , d_atlas(atlas)
{
}

Character Font::get(int id) const
{
    return d_characters.find(id)->second;
}
    
}