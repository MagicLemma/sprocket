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

    void Bind() const { d_atlas.Bind(); }
    void Unbind() const { d_atlas.Unbind(); }

    float TextWidth(const std::string& text, float size);

    std::shared_ptr<Texture> GetAtlas() const { return d_atlas.GetAtlas(); }
};

}
