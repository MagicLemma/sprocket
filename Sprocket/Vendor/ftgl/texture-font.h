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

struct TextureGlyph
{
    uint32_t codepoint;

    std::size_t width;
    std::size_t height;

    int offset_x;
    int offset_y;

    float advance_x;
    float advance_y;

    float s0;
    float t0;
    float s1;
    float t1;

    std::vector<Kerning> kerning;

    float outline_thickness;
};

struct texture_font_t
{
    FontAtlas* atlas;
    float size;
    std::string filename;
    
    std::vector<std::shared_ptr<TextureGlyph>> glyphs;
    int hinting;
    float outline_thickness;
    float height;
    float underline_position;
    float underline_thickness;
    int padding;
};


std::shared_ptr<texture_font_t>
texture_font_new_from_file(FontAtlas* atlas,
                           float size,
                           const std::string& filename);

std::shared_ptr<TextureGlyph>
texture_font_get_glyph(std::shared_ptr<texture_font_t> self,
                       const char* codepoint );

std::shared_ptr<TextureGlyph>
texture_font_find_glyph(std::shared_ptr<texture_font_t> self,
                        const char* codepoint );

int texture_font_load_glyph(std::shared_ptr<texture_font_t> self,
                            const char* codepoint );

float texture_glyph_get_kerning(const std::shared_ptr<TextureGlyph> self,
                                const char* codepoint );

std::shared_ptr<TextureGlyph> texture_glyph_new();

}
