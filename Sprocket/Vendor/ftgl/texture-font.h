#pragma once
#include <stdlib.h>
#include <stdint.h>
#include <cstddef>
#include <memory>
#include <vector>
#include <string>

#include "texture-atlas.h"

namespace Sprocket {

struct Kerning
{
    uint32_t codepoint;
    float kerning;
};

struct Glyph
{
    uint32_t codepoint = -1;

    std::size_t width  = 0;
    std::size_t height = 0;

    int offset_x = 0;
    int offset_y = 0;

    float advance_x = 0.0f;
    float advance_y = 0.0f;

    float s0 = 0.0f;
    float t0 = 0.0f;
    float s1 = 0.0f;
    float t1 = 0.0f;

    float outline_thickness = 0.0f;

    std::vector<Kerning> kerning;
};

class Font
{
    FontAtlas* d_atlas;

    std::string d_filename;
    float d_size;
    
    std::vector<std::shared_ptr<Glyph>> d_glyphs;
    float d_outline_thickness;
    float d_height;
    float d_underline_position;
    float d_underline_thickness;
    int d_padding;

    std::shared_ptr<Glyph> FindGlyph(char c);
    bool LoadGlyph(char c);

public:
    Font(FontAtlas* atlas);
    bool Load(const std::string& filename, float size);

    std::shared_ptr<Glyph> GetGlyph(char c);

    float GetKerning(const std::shared_ptr<Glyph> self, char c);
};

}
