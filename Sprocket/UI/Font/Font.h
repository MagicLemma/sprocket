#pragma once
#include <cstddef>
#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>

#include "FontAtlas.h"
#include "Maths.h"
#include "HashPair.h"
#include "Glyph.h"

namespace Sprocket {

using KerningMap = std::unordered_map<
    std::pair<uint32_t, uint32_t>,
    float,
    HashPair
>;

using GlyphMap = std::unordered_map<
    uint32_t,
    Glyph
>;

class Font
{
    FontAtlas d_atlas;

    std::string d_filename;
    float       d_size;
    
    GlyphMap   d_glyphs;
    KerningMap d_kernings;

    bool LoadGlyph(char c);

public:
    Font(const std::string& filename, float size);

    Glyph GetGlyph(char c);
    float GetKerning(char left, char right);

    void Bind() const { d_atlas.Bind(); }
    void Unbind() const { d_atlas.Unbind(); }

    float Size() const { return d_size; }
};

}
