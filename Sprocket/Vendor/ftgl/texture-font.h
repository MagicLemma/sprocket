#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <cstddef>
#include <memory>
#include <vector>
#include <string>

#include "vector.h"
#include "texture-atlas.h"

namespace ftgl {

enum class RenderMode
{
    RENDER_NORMAL,
    RENDER_OUTLINE_EDGE,
    RENDER_OUTLINE_POSITIVE,
    RENDER_OUTLINE_NEGATIVE
};


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

    std::vector<std::shared_ptr<Kerning>> kerning;

    RenderMode rendermode;

    float outline_thickness;

};

/**
 *  Texture font structure.
 */
struct texture_font_t
{
    /**
     * Vector of glyphs contained in this font.
     */
    std::vector<std::shared_ptr<TextureGlyph>> glyphs;

    /**
     * Atlas structure to store glyphs data.
     */
    std::shared_ptr<texture_atlas_t> atlas;

    std::string filename;

    /**
     * Font size
     */
    float size;

    /**
     * Whether to use autohint when rendering font
     */
    int hinting;

    /**
     * Mode the font is rendering its next glyph
     */
    RenderMode rendermode;

    /**
     * Outline thickness
     */
    float outline_thickness;

    /**
     * Whether to use our own lcd filter.
     */
    int filtering;

    /**
     * LCD filter weights
     */
    unsigned char lcd_weights[5];

    /**
     * Whether to use kerning if available
     */
    int kerning;


    /**
     * This field is simply used to compute a default line spacing (i.e., the
     * baseline-to-baseline distance) when writing text with this font. Note
     * that it usually is larger than the sum of the ascender and descender
     * taken as absolute values. There is also no guarantee that no glyphs
     * extend above or below subsequent baselines when using this distance.
     */
    float height;

    /**
     * This field is the distance that must be placed between two lines of
     * text. The baseline-to-baseline distance should be computed as:
     * ascender - descender + linegap
     */
    float linegap;

    /**
     * The ascender is the vertical distance from the horizontal baseline to
     * the highest 'character' coordinate in a font face. Unfortunately, font
     * formats define the ascender differently. For some, it represents the
     * ascent of all capital latin characters (without accents), for others it
     * is the ascent of the highest accented character, and finally, other
     * formats define it as being equal to bbox.yMax.
     */
    float ascender;

    /**
     * The descender is the vertical distance from the horizontal baseline to
     * the lowest 'character' coordinate in a font face. Unfortunately, font
     * formats define the descender differently. For some, it represents the
     * descent of all capital latin characters (without accents), for others it
     * is the ascent of the lowest accented character, and finally, other
     * formats define it as being equal to bbox.yMin. This field is negative
     * for values below the baseline.
     */
    float descender;

    /**
     * The position of the underline line for this face. It is the center of
     * the underlining stem. Only relevant for scalable formats.
     */
    float underline_position;

    /**
     * The thickness of the underline for this face. Only relevant for scalable
     * formats.
     */
    float underline_thickness;

    /**
    * The padding to be add to the glyph's texture that are loaded by this font.
    * Usefull when adding effects with shaders.
    */
    int padding;

};


std::shared_ptr<texture_font_t>
texture_font_new_from_file( std::shared_ptr<texture_atlas_t> atlas,
                        const float pt_size,
                        const char * filename );

std::shared_ptr<TextureGlyph>
texture_font_get_glyph( std::shared_ptr<texture_font_t> self,
                    const char * codepoint );

std::shared_ptr<TextureGlyph>
texture_font_find_glyph( std::shared_ptr<texture_font_t> self,
                    const char * codepoint );

int
texture_font_load_glyph( std::shared_ptr<texture_font_t> self,
                    const char * codepoint );

size_t
texture_font_load_glyphs(std::shared_ptr<texture_font_t> self,
                    const char * codepoints );


float texture_glyph_get_kerning(const std::shared_ptr<TextureGlyph> self,
                                const char* codepoint );

std::shared_ptr<TextureGlyph> texture_glyph_new();

}
