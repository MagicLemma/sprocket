#pragma once
#include "FontAtlas.h"
#include "Maths.h"
#include "HashPair.h"
#include "Glyph.h"
#include "Types.h"

#include <cstddef>
#include <memory>
#include <vector>
#include <string>
#include <utility>
#include <unordered_map>

namespace Sprocket {

using KerningMap = std::unordered_map<std::pair<u32, u32>, float, HashPair>;
using GlyphMap = std::unordered_map<u32, Glyph>;

struct SizedFontData
{
    GlyphMap   glyphs;
    KerningMap kernings;
};

class Font
{
    std::string d_filename;
    
    FontAtlas  d_atlas;
    std::unordered_map<float, SizedFontData> d_fontData;

    bool LoadGlyph(char c, float size);

public:
    Font(const std::string& filename);

    Glyph GetGlyph(char c, float size);
    float GetKerning(char left, char right, float size);

    void Bind(int slot) const { d_atlas.Bind(slot); }

    float TextWidth(std::string_view text, float size);

    Texture* GetAtlas() const { return d_atlas.GetAtlas(); }
};

}
