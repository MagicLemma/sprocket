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

#include "tsl/robin_map.h"

namespace Sprocket {

using KerningMap = tsl::robin_map<
    std::pair<uint32_t, uint32_t>,
    float,
    HashPair
>;

using GlyphMap = tsl::robin_map<
    uint32_t,
    Glyph
>;

struct SizedFontData
{
    GlyphMap   glyphs;
    KerningMap kernings;
};

class Font
{
    std::string d_filename;
    
    FontAtlas  d_atlas;
    tsl::robin_map<float, SizedFontData> d_fontData;

    bool LoadGlyph(char c, float size);

public:
    Font(const std::string& filename);

    Glyph GetGlyph(char c, float size);
    float GetKerning(char left, char right, float size);

    void Bind(int slot) const { d_atlas.Bind(slot); }

    float TextWidth(const std::string& text, float size);

    std::shared_ptr<Texture> GetAtlas() const { return d_atlas.GetAtlas(); }
};

}
