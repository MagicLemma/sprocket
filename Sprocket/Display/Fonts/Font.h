#pragma once
#include "Glyph.h"
#include "Texture.h"

#include <unordered_map>

namespace Sprocket {

using GlyphMap = std::unordered_map<int, GlyphStruct>;

enum class FontEnum
{
    ARIAL,
    GEORGIA,
    CALIBRI
};

class FontPackage
{
    Texture  d_atlas;
    GlyphMap d_glyphs; 
    float    d_size;

public:
    FontPackage(const std::string& fntFile,
                const std::string& texFile);

    GlyphStruct Get(int id) const;

    Texture Atlas() const { return d_atlas; }
    float Size() const { return d_size; }
};

}