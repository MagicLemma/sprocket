#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <cstddef>
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

    vector_t * kerning;

    RenderMode rendermode;

    float outline_thickness;

};

enum class FontLocation
{
    TEXTURE_FONT_FILE = 0,
    TEXTURE_FONT_MEMORY
};

/**
 *  Texture font structure.
 */
struct texture_font_t
{
    /**
     * Vector of glyphs contained in this font.
     */
    vector_t * glyphs;

    /**
     * Atlas structure to store glyphs data.
     */
    texture_atlas_t * atlas;

    /**
     * font location
     */
    FontLocation location;

    union {
        /**
         * Font filename, for when location == TEXTURE_FONT_FILE
         */
        char *filename;

        /**
         * Font memory address, for when location == TEXTURE_FONT_MEMORY
         */
        struct {
            const void *base;
            size_t size;
        } memory;
    };

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



/**
 * This function creates a new texture font from given filename and size.  The
 * texture atlas is used to store glyph on demand. Note the depth of the atlas
 * will determine if the font is rendered as alpha channel only (depth = 1) or
 * RGB (depth = 3) that correspond to subpixel rendering (if available on your
 * freetype implementation).
 *
 * @param atlas     A texture atlas
 * @param pt_size   Size of font to be created (in points)
 * @param filename  A font filename
 *
 * @return A new empty font (no glyph inside yet)
 *
 */
  texture_font_t *
  texture_font_new_from_file( texture_atlas_t * atlas,
                              const float pt_size,
                              const char * filename );


/**
 * This function creates a new texture font from a memory location and size.
 * The texture atlas is used to store glyph on demand. Note the depth of the
 * atlas will determine if the font is rendered as alpha channel only
 * (depth = 1) or RGB (depth = 3) that correspond to subpixel rendering (if
 * available on your freetype implementation).
 *
 * @param atlas       A texture atlas
 * @param pt_size     Size of font to be created (in points)
 * @param memory_base Start of the font file in memory
 * @param memory_size Size of the font file memory region, in bytes
 *
 * @return A new empty font (no glyph inside yet)
 *
 */
  texture_font_t *
  texture_font_new_from_memory( texture_atlas_t *atlas,
                                float pt_size,
                                const void *memory_base,
                                size_t memory_size );

/**
 * Delete a texture font. Note that this does not delete the glyph from the
 * texture atlas.
 *
 * @param self a valid texture font
 */
  void
  texture_font_delete( texture_font_t * self );


/**
 * Request a new glyph from the font. If it has not been created yet, it will
 * be.
 *
 * @param self      A valid texture font
 * @param codepoint Character codepoint to be loaded in UTF-8 encoding.
 *
 * @return A pointer on the new glyph or 0 if the texture atlas is not big
 *         enough
 *
 */
  TextureGlyph *
  texture_font_get_glyph( texture_font_t * self,
                          const char * codepoint );

/**
 * Request an already loaded glyph from the font.
 *
 * @param self      A valid texture font
 * @param codepoint Character codepoint to be found in UTF-8 encoding.
 *
 * @return A pointer on the glyph or 0 if the glyph is not loaded
 */
 TextureGlyph *
 texture_font_find_glyph( texture_font_t * self,
                          const char * codepoint );

/**
 * Request the loading of a given glyph.
 *
 * @param self       A valid texture font
 * @param codepoints Character codepoint to be loaded in UTF-8 encoding.
 *
 * @return One if the glyph could be loaded, zero if not.
 */
  int
  texture_font_load_glyph( texture_font_t * self,
                           const char * codepoint );

/**
 * Request the loading of several glyphs at once.
 *
 * @param self       A valid texture font
 * @param codepoints Character codepoints to be loaded in UTF-8 encoding. May
 *                   contain duplicates.
 *
 * @return Number of missed glyph if the texture is not big enough to hold
 *         every glyphs.
 */
  size_t
  texture_font_load_glyphs( texture_font_t * self,
                            const char * codepoints );

/*
 * Increases the size of a fonts texture atlas
 * Invalidates all pointers to font->atlas->data
 * Changes the UV Coordinates of existing glyphs in the font
 *
 * @param self A valid texture font
 * @param width_new Width of the texture atlas after resizing (must be bigger
 *                  or equal to current width)
 * @param height_new Height of the texture atlas after resizing (must be bigger or
 *                   equal to current height)
 */
void
texture_font_enlarge_atlas( texture_font_t * self, size_t width_new,
                            size_t height_new );

/**
 * Get the kerning between two horizontal glyphs.
 *
 * @param self      A valid texture glyph
 * @param codepoint Character codepoint of the peceding character in UTF-8 encoding.
 *
 * @return x kerning value
 */
float
texture_glyph_get_kerning( const TextureGlyph * self,
                           const char * codepoint );


/**
 * Creates a new empty glyph
 *
 * @return a new empty glyph (not valid)
 */
TextureGlyph *
texture_glyph_new( void );

/** @} */

}
