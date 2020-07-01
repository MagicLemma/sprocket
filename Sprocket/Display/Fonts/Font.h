#pragma once
#include "Glyph.h"
#include "Texture.h"

#include <unordered_map>

namespace Sprocket {

using GlyphMap = std::unordered_map<int, Glyph>;

enum class Font
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

    Glyph Get(int id) const;

    Texture Atlas() const { return d_atlas; }
    float Size() const { return d_size; }
};

}