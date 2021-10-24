#pragma once
#include <Sprocket/UI/Font/font_atlas.h>
#include <Sprocket/UI/Font/glyph.h>
#include <Sprocket/Utility/hashing.h>

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace spkt {

using codepoint_t = std::uint32_t;
using kerning_map = std::unordered_map<std::pair<codepoint_t, codepoint_t>, float, spkt::hash_pair>;
using glyph_map = std::unordered_map<codepoint_t, Glyph>;

struct sized_font_data
{
    glyph_map   glyphs;
    kerning_map kernings;
};

class font
{
    std::string d_filename;
    
    font_atlas  d_atlas;
    std::unordered_map<float, sized_font_data> d_font_data;

    bool load_glyph(char c, float size);

public:
    font(const std::string& filename);

    Glyph get_glyph(char c, float size);
    float get_kerning(char left, char right, float size);

    void bind(int slot) const { d_atlas.bind(slot); }

    float text_width(std::string_view text, float size);

    const texture* get_atlas() const { return d_atlas.get_atlas(); }
};

}
