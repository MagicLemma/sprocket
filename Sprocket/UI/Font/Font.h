#pragma once
#include <Sprocket/UI/Font/FontAtlas.h>
#include <Sprocket/UI/Font/Glyph.h>
#include <Sprocket/Utility/Hashing.h>

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace spkt {

using codepoint_t = std::uint32_t;
using KerningMap = std::unordered_map<std::pair<codepoint_t, codepoint_t>, float, spkt::hash_pair>;
using GlyphMap = std::unordered_map<codepoint_t, Glyph>;

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

    texture* GetAtlas() const { return d_atlas.GetAtlas(); }
};

}
