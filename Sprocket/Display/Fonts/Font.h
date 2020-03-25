#pragma once
#include "Character.h"
#include "Texture.h"

#include <unordered_map>

namespace Sprocket {

using CharacterMap = std::unordered_map<int, Character>;

class Font
{
    CharacterMap d_characters;
    Texture      d_atlas;

public:
    Font(const CharacterMap& characters, const Texture& atlas);

    Character get(int id) const;

    Texture atlas() const { return d_atlas; }
};

}