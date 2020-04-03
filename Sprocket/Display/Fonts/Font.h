#pragma once
#include "Character.h"
#include "Texture.h"
#include "ResourceManager.h"

#include <unordered_map>

namespace Sprocket {

using GlyphMap = std::unordered_map<int, Character>;

enum class Font
{
    ARIAL,
    GEORGIA,
    CALIBRI
};

std::string getFontName(Font font);

class FontPackage
{
    std::string d_name;

    Texture  d_atlas;
    GlyphMap d_glyphs; 
    float    d_size;

public:
    FontPackage(ResourceManager* resourceManager,
                const std::string& name,
                const std::string& fntFile,
                const std::string& texFile);

    Character get(int id) const;

    Texture atlas() const { return d_atlas; }
    float size() const { return d_size; }
};

}