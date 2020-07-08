/* Freetype GL - A C OpenGL Freetype engine
 *
 * Distributed under the OSI-approved BSD 2-Clause License.  See accompanying
 * file `LICENSE` for more details.
 */
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_STROKER_H
// #include FT_ADVANCES_H
#include FT_LCD_FILTER_H
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <algorithm>
#include "texture-font.h"
#include "utf8-utils.h"
#include "Maths.h"

#define HRES  64
#define HRESf 64.f
#define DPI   72

namespace Sprocket {
namespace {

bool LoadFace(
    const std::string& filename,
    float size,
    FT_Library *library,
    FT_Face *face)
{
    FT_Error error;
    FT_Matrix matrix = {
        (int)((1.0/HRES) * 0x10000L),
        (int)((0.0)      * 0x10000L),
        (int)((0.0)      * 0x10000L),
        (int)((1.0)      * 0x10000L)};

    assert(library);
    assert(size);

    error = FT_Init_FreeType(library);
    if (error) {
        return false;
    }

    error = FT_New_Face(*library, filename.c_str(), 0, face);
    if (error) {
        FT_Done_FreeType(*library);
        return false;
    }

    error = FT_Select_Charmap(*face, FT_ENCODING_UNICODE);
    if (error) {
        FT_Done_Face(*face);
        FT_Done_FreeType(*library);
        return false;
    }

    error = FT_Set_Char_Size(*face, (int)(size * HRES), 0, DPI * HRES, DPI);
    if (error) {
        FT_Done_Face(*face);
        FT_Done_FreeType(*library);
        return false;
    }

    FT_Set_Transform(*face, &matrix, NULL);
    return true;
}

void GenerateKerning(
    std::vector<std::shared_ptr<Glyph>>& glyphs,
    FT_Library* library,
    FT_Face* face)
{
    // For each glyph couple combination, check if kerning is necessary
    // Starts at index 1 since 0 is for the special backgroudn glyph
    for (std::size_t i = 1; i < glyphs.size(); ++i) {
        auto glyph = glyphs[i];
        FT_UInt glyph_index = FT_Get_Char_Index( *face, glyph->codepoint );
        glyph->kerning.clear();

        for (std::size_t j = 1; j < glyphs.size(); ++j) {
            auto prev_glyph = glyphs[j];
            FT_UInt prev_index = FT_Get_Char_Index(*face, prev_glyph->codepoint);
            FT_Vector kerning;
            FT_Get_Kerning(*face, prev_index, glyph_index, FT_KERNING_UNFITTED, &kerning );

            if (kerning.x) {
                Kerning k;
                k.codepoint = prev_glyph->codepoint;
                k.kerning = kerning.x / (float)(HRESf*HRESf);
                glyph->kerning.push_back(k);
            }
        }
    }
}

}

Font::Font(std::size_t width, std::size_t height)
    : d_atlas(width, height)
    , d_outline_thickness(0.0f)
    , d_padding(1)
{
}

bool Font::Load(const std::string& filename, float size)
{
    assert(size);
    
    d_filename = filename;
    d_size = size;

    FT_Library library;
    FT_Face face;

    if (!LoadFace(filename, size, &library, &face)) {
        return false;
    }

    d_underline_position = face->underline_position / (float)(HRESf*HRESf) * d_size;
    d_underline_position = roundf(d_underline_position);
    if (d_underline_position > -2) {
        d_underline_position = -2.0;
    }

    d_underline_thickness = face->underline_thickness / (float)(HRESf*HRESf) * d_size;
    d_underline_thickness = roundf(d_underline_thickness);
    if (d_underline_thickness < 1) {
        d_underline_thickness = 1.0;
    }

    FT_Size_Metrics metrics = face->size->metrics;
    d_height = (metrics.height >> 6) / 100.0;
    FT_Done_Face(face);
    FT_Done_FreeType(library);
    return true;
}

std::shared_ptr<Glyph> Font::GetGlyph(char c)
{
    auto glyph = FindGlyph(c);
    if (glyph != nullptr) {
        return glyph;
    }

    LoadGlyph(c);
    return FindGlyph(c);
}

std::shared_ptr<Glyph> Font::FindGlyph(char c)
{
    uint32_t ucodepoint = utf8_to_utf32(&c);
    for (std::size_t i = 0; i < d_glyphs.size(); ++i) {
        auto glyph = d_glyphs[i];
        if (glyph->codepoint == ucodepoint) {
            return glyph;
        }
    }

    return nullptr;
}

bool Font::LoadGlyph(char c)
{
    if (FindGlyph(c)) {
        return true; // Already loaded
    }

    FT_Library library;
    FT_Face face;

    if (!LoadFace(d_filename, d_size, &library, &face)) {
        return false;
    }

    Maths::ivec4 region;
    size_t missed = 0;

    if (!LoadFace(d_filename, d_size, &library, &face)) {
        return false;
    }

    FT_UInt glyph_index = FT_Get_Char_Index(face, (FT_ULong)utf8_to_utf32(&c));
    FT_Int32 flags = FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT;

    if (FT_Load_Glyph(face, glyph_index, flags)) {
        FT_Done_Face(face);
        FT_Done_FreeType(library);
        return false;
    }

    FT_GlyphSlot slot   = face->glyph;
    FT_Bitmap ft_bitmap = slot->bitmap;
    int ft_glyph_top    = slot->bitmap_top;
    int ft_glyph_left   = slot->bitmap_left;

    struct {
        int left;
        int top;
        int right;
        int bottom;
    } padding = { 0, 0, 1, 1 };

    if (d_padding != 0) {
        padding.top += d_padding;
        padding.left += d_padding;
        padding.right += d_padding;
        padding.bottom += d_padding;
    }

    std::size_t src_w = ft_bitmap.width;
    std::size_t src_h = ft_bitmap.rows;

    std::size_t tgt_w = src_w + padding.left + padding.right;
    std::size_t tgt_h = src_h + padding.top + padding.bottom;

    region = d_atlas.GetRegion(tgt_w, tgt_h);

    if (region.x < 0) {
        SPKT_LOG_ERROR("Texture atlas is full!");
        FT_Done_Face( face );
        FT_Done_FreeType( library );
        return 0;
    }

    std::size_t x = region.x;
    std::size_t y = region.y;

    std::vector<unsigned char> buffer;
    buffer.resize(tgt_w * tgt_h);
    
    unsigned char *dst_ptr = buffer.data() + (padding.top * tgt_w + padding.left);
    unsigned char *src_ptr = ft_bitmap.buffer;
    for (std::size_t i = 0; i < src_h; i++ )
    {
        std::memcpy( dst_ptr, src_ptr, ft_bitmap.width);
        dst_ptr += tgt_w;
        src_ptr += ft_bitmap.pitch;
    }

    d_atlas.SetRegion({x, y, tgt_w, tgt_h}, buffer);

    auto glyph = std::make_shared<Glyph>();
    glyph->codepoint = utf8_to_utf32(&c);
    glyph->width    = tgt_w;
    glyph->height   = tgt_h;
    glyph->offset_x = ft_glyph_left;
    glyph->offset_y = ft_glyph_top;
    glyph->s0       = x/(float)d_atlas.Width();;
    glyph->t0       = y/(float)d_atlas.Height();
    glyph->s1       = (x + glyph->width)/(float)d_atlas.Width();
    glyph->t1       = (y + glyph->height)/(float)d_atlas.Height();
    glyph->outline_thickness = d_outline_thickness;

    // Discard hinting to get advance
    FT_Load_Glyph(face, glyph_index, FT_LOAD_RENDER | FT_LOAD_NO_HINTING);
    slot = face->glyph;
    glyph->advance_x = slot->advance.x / HRESf;
    glyph->advance_y = slot->advance.y / HRESf;

    d_glyphs.push_back(glyph);

    GenerateKerning(d_glyphs, &library, &face);

    FT_Done_Face(face);
    FT_Done_FreeType(library);
    return true;
}

float Font::GetKerning(
    const std::shared_ptr<Glyph> glyph,
    char c)
{
    uint32_t ucodepoint = utf8_to_utf32(&c);
    auto& k = glyph->kerning;

    auto it = std::find_if(k.begin(), k.end(), [&](const auto x) {
        return x.codepoint == ucodepoint;
    });

    if (it != k.end()) {
        return it->kerning;
    }

    return 0;
}

}